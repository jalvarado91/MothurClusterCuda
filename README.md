# MothurClusterCuda

This is a distilled Mothur build. The focus is to run the mothur cluster command and paralellize 
the `SparseDistanceMatrix::getSmallestCell` method using CUDA. 

The `mothur.cpp` file instantianes the Cluster command with two input files. A sequence distance matrix `sogin.unique.filter.dist` file
and a sequence name file `sogin.names`. The output files are three cluster files in list, rabund and sabund formats. 

The makefile builds both c++ and .cu files and links them together simply by running:

    $ make
    
and running the program

    $ ./mothurcluster
    
