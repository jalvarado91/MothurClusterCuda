# MothurClusterCuda

This is a distilled Mothur build. The focus is to run the mothur cluster command and paralellize 
the `SparseDistanceMatrix::getSmallestCell` method using CUDA. 

The `mothur.cpp` file instantianes the Cluster command with two input files. A sequence distance matrix `sogin.unique.filter.dist` file
and a sequence name file `sogin.names`. The output files are three cluster files in list, rabund and sabund formats. 

The makefile builds both c++ and .cu files and links them together simply by running:

    $ make
    
and running the program

    $ ./mothurcluster
    

## Branches
The `master` branch contains the isolated command currently compiling with nvcc. 
The `paralellize` branch contains an effort to parallelize the `SparseDistanceMatrix::getSmallestCell`.
The `template` branch is meant to serve as a starting point to develop CUDA and C/C++ content that works together and compiles with nvcc
