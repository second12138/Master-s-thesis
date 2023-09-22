#include <sdsl/wavelet_trees.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <cstdlib>
#include <bits/stdc++.h>
#include <cmath>
#include <sys/mman.h>
#include <ctime>
#include <ratio>
#include <chrono>

/*
@author:Yansong Li
start from: 2022/06/17
BWT experiment phase 3 code
*/

using namespace sdsl;
using namespace std;
using namespace std::chrono;

//a struct to store a block and its header
struct Block{
    wt_huff<rrr_vector<63>> first;
    long long* second;
};

//access queries 
char access(long long* P, Block* T, int B, long long I){
    return T[P[I/B]].first[I%B];
}

//rank queries
long long  Rank(long long* P, long long R, char R_C, Block* T, unordered_map<char, int> A){
    long long curr_block=R/T[0].first.size();
    long long curr_offset=R%T[0].first.size();
    if(curr_block==0){
        return T[P[curr_block]].first.rank(curr_offset, R_C);
    }else{
        return T[P[curr_block]].second[A[R_C]]+T[P[curr_block]].first.rank(curr_offset, R_C);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        return 1;
    }
    //file1 is the permutation file
    string file1 = argv[1];
    //file2 is the file contains all the block wavlet tree
    string file2 = argv[2];
    //read in the input string file, mostly BWTs
    wt_huff<rrr_vector<63>> string_wt;
    load_from_file(string_wt, "pine_genome_wt.sdsl");
    
    //read in the permutation file, a permutation of a blocked string
    fstream per_file;
    per_file.open(file1, ios::in);
    long long block;
    per_file >> block;
    long long length=string_wt.size();
    cout<<"The length of your string is "<<length<<" and the block size is "<<block<<endl;
    long long per_len=0;
    if(length%block==0){
        per_len=length/block;
    }else{
        per_len=length/block+1;
    }
    long long* permutation = new long long [per_len];
    long long n=0;
    while(!per_file.eof()){
        per_file >> permutation[n];
        permutation[n]--;
        n++;
    }
    per_file.close();

    //read in the ranks file that contains the rank of each character when the BWT is alphabetically sorted
    fstream rank_file;
    rank_file.open("pine_genome_ranks.txt");
    string line;
    int al_size=0;
    unordered_map<char, long long> char_first;
    string alphabet;
    while(getline(rank_file, line)){
	    al_size++;
        char* str = &line[0];
        char* character;
        character = strtok(str, "\t");
        char* rank;
        rank = strtok(NULL, "\t");
        char_first[(char)stoi(character)]=stoll(rank);
        alphabet.push_back((char)stoi(character));
    }
    rank_file.close();
    sort(alphabet.begin(), alphabet.end());
    unordered_map<char, int> alphabet_index;
    for(int i=0; i<al_size; i++){
        alphabet_index[alphabet[i]]=i;
    }
    //number of blocks
    long long block_num=per_len;
    //starts to record the time for building the table
    clock_t start, end;
    //build the table of blocks and headers
    start=clock();
    //high_resolution_clock::time_point t1 = high_resolution_clock::now();
    Block* table = new Block[block_num];
    ifstream in(file2);
    for(int i=0; i<block_num; i++){
        wt_huff<rrr_vector<63>> wt;
        load(wt, in);
        long long* rank = new long long [al_size];
        for(int j=0; j<al_size; j++){
            rank[j]=string_wt.rank(i*block, alphabet[j]);
        }      
        table[permutation[i]]={wt, rank};
    }
    in.close();
    //high_resolution_clock::time_point t2 = high_resolution_clock::now();
    //duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    //cout << "Time taken by building the table is : " << time_span.count() << " seconds."<<endl;
    end=clock();
    double time_taken = double(end - start)/ double(CLOCKS_PER_SEC);
    cout << "Time taken by building the table is : " << fixed
    << time_taken << setprecision(5);
    cout << " sec " << endl;
    //the length of the extraction and the starting point
    long long ex=10000;
    long long old_p=ex;
    long long new_p=0;
    
    //record the time of sdsl extraction
    start=clock();
    char* a = new char[ex];
    //high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for(long long i=0; i<ex; i++){
        char ac = string_wt[old_p];
        a[i]=ac;
        new_p=char_first[ac]+string_wt.rank(old_p, ac);
        old_p=new_p;
    }
    //high_resolution_clock::time_point t2 = high_resolution_clock::now();
    //duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    //cout << "Time taken by reversing the BWT to text using wavelet tree is : " << time_span.count() << " seconds."<<endl;
    end=clock();
    time_taken = double(end - start)/ double(CLOCKS_PER_SEC);
    cout << "Time taken by reversing the BWT to text using wavelet tree is : " << fixed
    << time_taken << setprecision(5);
    cout << " sec " << endl;
    ofstream txt_result;
    txt_result.open("pine_genome_extraction_sdsl.txt");
    for(long long i=ex-1; i>=0; i--){
        txt_result<<a[i];
    }
    txt_result.close();
    old_p=ex;
    new_p=0;
    //record the time of data structure extraction
    char* a1 = new char[ex];
    start=clock();
    //t1 = high_resolution_clock::now();
    for(long long i=0; i<ex; i++){
        char ac = access(permutation, table, block, old_p);
        long long o=(permutation[old_p/block])*block+(old_p%block);
        a1[i]=ac;
        new_p=char_first[ac]+Rank(permutation, old_p, ac, table, alphabet_index);
        long long n=(permutation[new_p/block])*block+(new_p%block);
        old_p=new_p; 
    }
    //t2 = high_resolution_clock::now();
    //time_span = duration_cast<duration<double>>(t2 - t1);
    //cout << "Time taken by reversing the BWT to text using data structure is : " << time_span.count() << " seconds."<<endl;
    end=clock();
    time_taken = double(end - start)/ double(CLOCKS_PER_SEC);
    cout << "Time taken by reversing the BWT to text using data structure is : " << fixed
    << time_taken << setprecision(5);
    cout << " sec " << endl;

    ofstream txt_result1;
    txt_result1.open("pine_genome_extraction_mine.txt");
    for(long long i=ex-1; i>=0; i--){
        txt_result1<<a1[i];
    }    
    txt_result1.close();
    return 0;
}
