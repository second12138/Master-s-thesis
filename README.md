# Master's Thesis of Yansong Li
This is the repository for Yansng Li's thesis for the Master of Computer Science at Dalhousie University (Halifax, NS, Canada).

## A copy of the original thesis 
The pdf file named YansongLi2023 is a copy of the thesis, which can also be found at
```bash
http://hdl.handle.net/10222/82808
```

## Datasets used in the experiments
1. (Prime) The Douglas Fir Genome
```bash
https://www.ncbi.nlm.nih.gov/datasets/taxonomy/3357/
```

2. The Human Reference Genome
```bash
https://www.ncbi.nlm.nih.gov/datasets/genome/GCF_000001405.40/
```

## Software used in the experiments
1. SDSL used to building BWT and wavelet trees of the dataset.
```bash
https://github.com/simongog/sdsl-lite
```

2. Metis Clustering used to cluster the BWT blocks
```bash
http://glaros.dtc.umn.edu/gkhome/metis/metis/download
```

3. Gurobi Optimizer used to optimizer the layout of the BWT blocks inside clusters
```bash
https://www.gurobi.com/downloads/gurobi-software/
```

## Re-run the experiments
1. Build the BWT graph and BWT of the dataset.
```bash
buildGraph.cpp
```

2. Cluster the BWT graph using Metis.


3. Build the permutation of blocks using the output of Metis and store it to a file starting with stating the block size.
```bash
Example of the format of the permutation file: pine_genome_1000000_metisOrder10.txt (clustered with block size 100000 bases and cluster number of 10)
```

4. Optimize the layout of BWT blocks inside each cluster (optional).
```bash
cluster.py (it can only optimize the layout of one cluster each time).
```

5. Build wavelet tree of the BWT of the dataset.
```bash
string_wt.cpp
```

6. Build wavelet forest of the permuted BWT blocks of the dataset.
```bash
short_wt.cpp
```

7. Calculate the ranks of each characters in the alphabet of the alphabetically sorted dataset.
```bash
ranks.cpp
```

8. Substract a certain length of snippet of the dataset using both default layout of the BWT and optimized layout then compare their running times.
```bash
inverse.cpp
```

## Maintainer
This project is mantained by:
* [Yansong Li](https://git.cs.dal.ca/yansong)
