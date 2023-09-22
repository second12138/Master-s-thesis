#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include <random>

/*
@author:Yansong Li
start from: 2022/06/17
BWT experiment phase 3 code
*/

using namespace std;
string slurp( string const& filename )
{
    using BufIt = istreambuf_iterator<char>;
    ifstream in( filename );
    return string( BufIt( in.rdbuf() ), BufIt() );
}

int main() {
    //read in the input string file, mostly BWTs
    auto input = slurp( "pine_genome_BWT.txt" );
    long long length = input.size();
    string c= input;
    bool count[256];
    memset(count, 0, 256 * sizeof(bool));
    int al_size=0;
    for (long long i = 0; i < length; i++) {
        if(count[(int)c[i]]==0){
            count[(int)c[i]]=1;
            al_size++;
        }
    }
    sort(c.begin(), c.end());
    unordered_map<char, long long> char_first;
    for(int i=0; i<256; i++){
        if(count[i]!=0){
            char_first[char(i)] = c.find(char(i));
        }
    }
    ofstream out_file;
    out_file.open("pine_genome_ranks.txt");
    for(auto it=char_first.begin(); it!=char_first.end(); it++){
        out_file<<(int) it->first<<"\t"<<it->second<<endl;
    }
}