#include<stdio.h>
#include<stdlib.h>
#include <mpi.h>

void barrier(int mytid, int nproc,MPI_Comm comm);

void main(int argc, char  **argv){
	int mytid, nproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &mytid);
    barrier(mytid, nproc,MPI_COMM_WORLD);
    MPI_Finalize();
}

void barrier(int mytid, int nproc,MPI_Comm comm){
	int i;
	MPI_Status status;
	if (mytid==0){
    	for (i = 1; i < nproc; i++){
			MPI_Recv(MPI_ARGV_NULL, 0, MPI_BYTE, i, 99, comm, &status);
		}
		for (i = 1; i < nproc; i++){
			MPI_Send(MPI_ARGV_NULL, 0, MPI_BYTE, i, 99, comm);
		}
    }
    else{
    	MPI_Send(MPI_ARGV_NULL, 0, MPI_BYTE, 0, 99, comm);
		MPI_Recv(MPI_ARGV_NULL, 0, MPI_BYTE, 0, 99, comm, &status);
    }
	
}

