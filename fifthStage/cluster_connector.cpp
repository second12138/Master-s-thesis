#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

//a small piece of code that connect the clusters from metis.
int main() {
    //reads in what are the blocks inside current cluster
    vector<int> edge_member;
    for(int i=0; i<10; i++){
        fstream cluster_file;
        string line;
        string a = "pine_genome_cluster";
        string c = "_1500.txt";
        string b = to_string(i);
        cluster_file.open(a+b+c, ios::in);
        while(getline(cluster_file, line)){
            edge_member.push_back(stoi(line));
        }
        
    }
    cout<<edge_member.size()<<endl;
    ofstream block_result;
    block_result.open("pine_genome_1500_metisOrder10.txt");
    block_result<<"1500"<<endl;
    for(int i=0; i<edge_member.size(); i++){
        block_result <<edge_member[i]<<endl;
    }
    sort(edge_member.begin(), edge_member.end());
    cout<<edge_member[0]<<" "<<edge_member[edge_member.size()-1]<<endl;
    for(int i=0; i<edge_member.size(); i++){
        //cout<<edge_member[i]<<endl;
    }
    
}