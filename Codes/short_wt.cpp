#include <sdsl/wavelet_trees.hpp>
#include <fstream>
#include <string>
/*
@author:Yansong Li
start from: 2022/06/17
BWT experiment phase 3 code
*/

using namespace sdsl;
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
    fstream per_file;
    //read the permutation of the block wavelet trees
    per_file.open("pine_genome_9000000_metisOrder10.txt", ios::in);
    long long block;
    per_file >> block;
    long long block_num;
    if(input.size()%block==0){
        block_num=input.size()/block;
    }else{
        block_num=input.size()/block+1;
    }
    //output the block wavlet trees to a file.
    ofstream out("pine_genome_block_wt_9000000.sdsl");
    for(long long i=0; i<block_num; i++){
        string curr_sub=input.substr(i*block, block);
        wt_huff<rrr_vector<63>> wt; //uncompressed fixed block boosting wavelet tree
        //construct the wavlet tree from 
        construct_im(wt, curr_sub, 1);
        serialize(wt, out);
    }
}