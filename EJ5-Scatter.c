#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>

void Scatter(int mytid, int nproc, int root,MPI_Comm comm);

void main(int argc, char  **argv){
	int mytid, nproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &mytid);
    Scatter(mytid, nproc,atoi(argv[1]),MPI_COMM_WORLD);
    MPI_Finalize();
}

void Scatter(int mytid, int nproc, int root, MPI_Comm comm){
	int i, datorec;
	MPI_Status status;
	if (mytid==root){
        srand (time(NULL));
        int *dato = (int*)malloc(sizeof(int)*nproc);
    	for (i = 0; i < nproc; ++i){
            dato[i]= rand()%(9) + 1;
    		if(i!=root){
                MPI_Send(&dato[i], 1, MPI_INT, i, 99, comm);
    		}
            else{
                datorec=dato[i];
            }
    	}
        free(dato);
    }
    else{
		MPI_Recv(&datorec, 1, MPI_INT, root, 99, comm, &status);
    }
}
