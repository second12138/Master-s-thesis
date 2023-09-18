#include <iostream>
#include <cstdlib>
#include <sdsl/suffix_arrays.hpp>
#include <bits/stdc++.h>

//This parameter is used to define the size of the BWT blocks by bases
#define BLOCKSIZE 1500000

using namespace std;
using namespace sdsl;

typedef struct node {
    int target;
    int weight;
    struct node *next;
} node;

int edgeCount = 0;


void updateEdge(node **head, int target, int increase, int flag) {
    
    node *p = *head;
    
    while (p != NULL && p -> target != target) {
        p = p -> next;
    }
    
    if (p == NULL) {
        p = (node *) malloc(sizeof(node));
    	p -> target = target;
    	p -> weight = 0;
    	p -> next = *head;
    	*head = p;
    	
        edgeCount += flag;
    }
    
    p -> weight += increase;
}


int main(int argc, char* argv[]) {
    clock_t start, end,start1, end1;
    start1=clock();
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " file" << endl;
        return 1;
    }
    
    string file = argv[1];
    //starts to record the time for building the table
    start=clock();
    cache_config cc(true);
    csa_wt<> csa;
    construct(csa, file, 1);
    end=clock();
    double time_taken = double(end - start)/ double(CLOCKS_PER_SEC);
    cout << "Time taken by building the csa is : " << fixed
    << time_taken << setprecision(5);
    cout << " sec " << endl;

    long long n = csa.size();
    
    long long count[256];
    memset(count, 0, 256 * sizeof(long long));
    for (long long i = 0; i < n; i++) {
        count[(int)csa.bwt[i]]++;
    }

    long long C[256];
    memset(C, 0, 256 * sizeof(long long));
    C[0] = 0;
    for (int i = 1; i < 256; i++) {
        C[i] = C[i - 1] + count[i - 1];
    }
    
    long long rank[256];
    memset(rank, 0, 256 * sizeof(long long));

    int blockCount = (n + BLOCKSIZE - 1) / BLOCKSIZE;
    
    node **dTable = (node **) malloc(blockCount * sizeof(node *));
    memset(dTable, 0, blockCount * sizeof(node *));
    
    for (long long i = 0; i < n; i++) {
        
        int c = (int) csa.bwt[i];
        int target = (C[c] + rank[c]) / BLOCKSIZE;
        rank[c]++;
        
        if (target != i / BLOCKSIZE) {
            updateEdge(&dTable[i / BLOCKSIZE], target, 1, 0);
        }    
    }
    
    node **uTable = (node **) malloc(blockCount * sizeof(node *));
    memset(uTable, 0, blockCount * sizeof(node *));
    
    for (int i = 0; i < blockCount; i++) {
        
        node *p = dTable[i];
        
        while (p != NULL) {
            
            updateEdge(&uTable[i], p -> target, p -> weight, 1);
            updateEdge(&uTable[p -> target], i, p -> weight, 1);
            
            p = p -> next;
        }
    }
    ofstream metis_result;
    //Output the BWT graph for Metis clustering, in this case the block size is 1500000 bases.
    metis_result.open ("pine_genome_metis_1500000.txt");
    metis_result << blockCount << " " << edgeCount / 2 <<" 001"<< endl;
    long long totalWeight=0;
    for (int i = 0; i < blockCount; i++) {
        
        node *p = uTable[i];
        
        while (p != NULL) {
            metis_result << (p -> target) + 1 << " " << p -> weight << " ";
            totalWeight+=p -> weight;
            p = p -> next;
        }
        
        metis_result << endl;
    }
    cout<<"totalWeight: "<<totalWeight<<endl;
    metis_result.close();
    ofstream BWT_result;
    //Output the BWT of the input to a file.
    BWT_result.open("pine_genome_BWT.txt");
    for(long long i=0; i<n; i++){
		BWT_result<<csa.bwt[i];
	}  
    BWT_result.close();
    end1=clock();
    double time_taken1 = double(end1 - start1)/ double(CLOCKS_PER_SEC);
    cout << "Time taken by running the program is : " << fixed
    << time_taken1 << setprecision(5);
    cout << " sec " << endl;
    return 0;
}
