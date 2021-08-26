#include<stdio.h>
#include<stdlib.h>
#include <mpi.h>

void Bcast(int dato,int mytid, int nproc, int root,MPI_Comm comm);

void main(int argc, char  **argv){
	int mytid, nproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &mytid);
    Bcast(atoi(argv[1]),mytid, nproc,atoi(argv[2]),MPI_COMM_WORLD);
    MPI_Finalize();
}

void Bcast(int dato, int mytid, int nproc, int root, MPI_Comm comm){
	int i, datorec;
	MPI_Status status;
	if (mytid==root){
    	for (i = 0; i < nproc; ++i)
    	{
    		if(i!=root){
    			MPI_Send(&dato, 1, MPI_INT, i, 99, comm);
    		}
    	}
    	datorec=dato;
    }
    else{
		MPI_Recv(&datorec, 1, MPI_INT, root, 99, comm, &status);
    }
}
