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
    //constuct a wavelet tree of the input string S
    wt_huff<rrr_vector<63>> string_wt;
    construct_im(string_wt, input, 1);
    store_to_file(string_wt, "pine_genome_bwt.sdsl");
}