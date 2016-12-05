//
//  sparsedistancematrix.cpp
//  Mothur
//
//  Created by Sarah Westcott on 7/16/12.
//  Copyright (c) 2012 Schloss Lab. All rights reserved.
//

#include "sparsedistancematrix.h"
#include "getsmallestcellgpu.cuh"


/***********************************************************************/

SparseDistanceMatrix::SparseDistanceMatrix() : numNodes(0), smallDist(1e6){  m = MothurOut::getInstance(); sorted=false; aboveCutoff = 1e6; }

/***********************************************************************/

int SparseDistanceMatrix::getNNodes(){
	return numNodes; 
}
/***********************************************************************/

void SparseDistanceMatrix::clear(){
    for (int i = 0; i < seqVec.size(); i++) {  seqVec[i].clear();  }
    seqVec.clear();
}

/***********************************************************************/

float SparseDistanceMatrix::getSmallDist(){
	return smallDist;
}
/***********************************************************************/

int SparseDistanceMatrix::updateCellCompliment(ull row, ull col){
    try {
        
        ull vrow = seqVec[row][col].index;
        ull vcol = 0;
        
        //find the columns entry for this cell as well
        for (int i = 0; i < seqVec[vrow].size(); i++) {  
            if (seqVec[vrow][i].index == row) { vcol = i;  break; }  
        }
       
        seqVec[vrow][vcol].dist = seqVec[row][col].dist;
        
        return 0;
    }
	catch(exception& e) {
		m->errorOut(e, "SparseDistanceMatrix", "updateCellCompliment");
		exit(1);
	}
}
/***********************************************************************/

int SparseDistanceMatrix::rmCell(ull row, ull col){
	try {
        numNodes-=2;
 
        ull vrow = seqVec[row][col].index;
        ull vcol = 0;
        
        //find the columns entry for this cell as well
        for (int i = 0; i < seqVec[vrow].size(); i++) {  if (seqVec[vrow][i].index == row) { vcol = i;  break; }  }
        
        seqVec[vrow].erase(seqVec[vrow].begin()+vcol);
        seqVec[row].erase(seqVec[row].begin()+col);
 
		return(0);
    }
	catch(exception& e) {
		m->errorOut(e, "SparseDistanceMatrix", "rmCell");
		exit(1);
	}
}
/***********************************************************************/
void SparseDistanceMatrix::addCell(ull row, PDistCell cell){
	try {
		numNodes+=2;
		if(cell.dist < smallDist){ smallDist = cell.dist; }
        
        seqVec[row].push_back(cell);
        PDistCell temp(row, cell.dist);
        seqVec[cell.index].push_back(temp);
	}
	catch(exception& e) {
		m->errorOut(e, "SparseDistanceMatrix", "addCell");
		exit(1);
	}
}
/***********************************************************************/
int SparseDistanceMatrix::addCellSorted(ull row, PDistCell cell){
	try {
		numNodes+=2;
		if(cell.dist < smallDist){ smallDist = cell.dist; }
        
        seqVec[row].push_back(cell);
        PDistCell temp(row, cell.dist);
        seqVec[cell.index].push_back(temp);
        
        sortSeqVec(row);
        sortSeqVec(cell.index);
        
        int location = -1; //find location of new cell when sorted
        for (int i = 0; i < seqVec[row].size(); i++) {  if (seqVec[row][i].index == cell.index) { location = i; break; } }
        
        return location;
	}
	catch(exception& e) {
		m->errorOut(e, "SparseDistanceMatrix", "addCellSorted");
		exit(1);
	}
}

/***********************************************************************/

ull SparseDistanceMatrix::getSmallestCell(ull& row){
	try {
        if (!sorted) { sortSeqVec(); sorted = true; }
        
        int rows = seqVec.size();
        int cols = seqVec[0].size();


        vector<PDistCellMin> mins;
        smallDist = 1e6;
       
        PDistCell* h_seqVec = (PDistCell*)malloc(sizeof(PDistCell));  
        PDistCell* d_seqVec;  
        cudaMalloc((void**)&d_seqVec,sizeof(PDistCell));  
        cudaMemcpy(d_seqVec,h_seqVec,sizeof(PDistCell),cudaMemcpyHostToDevice);  
        dim3 dimblock(rows,rows);  
        dim3 dimgrid(cols,cols);  
        find_min<<<dimgrid,dimblock>>>(seqVec, mins);  
        cudaMemcpy(a,d_a,sizeof(sizeof(PDistCell)),cudaMemcpyDeviceToHost);    

        free(h_seqVec);
        cudaFree( d_seqVec );
        
		random_shuffle(mins.begin(), mins.end());  //randomize the order of the iterators in the mins vector
        
        row = mins[0].row;
        ull col = mins[0].col;

		return col;
	}
	catch(exception& e) {
		m->errorOut(e, "SparseDistanceMatrix", "getSmallestCell");
		exit(1);
	}
}
/***********************************************************************/

int SparseDistanceMatrix::sortSeqVec(){
	try {
        
        //saves time in getSmallestCell, by making it so you dont search the repeats
        for (int i = 0; i < seqVec.size(); i++) {  sort(seqVec[i].begin(), seqVec[i].end(), compareIndexes); }
    
        return 0;
    }
	catch(exception& e) {
		m->errorOut(e, "SparseDistanceMatrix", "sortSeqVec");
		exit(1);
	}
}
/***********************************************************************/

int SparseDistanceMatrix::sortSeqVec(int index){
	try {
        
        //saves time in getSmallestCell, by making it so you dont search the repeats
        sort(seqVec[index].begin(), seqVec[index].end(), compareIndexes);
        
        return 0;
    }
	catch(exception& e) {
		m->errorOut(e, "SparseDistanceMatrix", "sortSeqVec");
		exit(1);
	}
}
/***********************************************************************/

