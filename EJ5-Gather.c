#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>

void Gather(int mytid, int nproc, int root,MPI_Comm comm);

void main(int argc, char  **argv){
	int mytid, nproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &mytid);
    Gather(mytid, nproc,atoi(argv[1]),MPI_COMM_WORLD);
    MPI_Finalize();
}

void Gather(int mytid, int nproc, int root, MPI_Comm comm){
	int i, datoRoot=0;
	MPI_Status status;
	if (mytid==root){
        int *datorec = (int*)malloc(sizeof(int)*nproc);
    	for (i = 0; i < nproc; ++i)
    	{
    		if(i!=root){
    			MPI_Recv(&datorec[i], 1, MPI_INT, i, 99, comm, &status);
    		}
            else{
                datorec[i]=datoRoot;
            }
    	}
        free(datorec);
    }
    else{
        srand (time(NULL)); 
        int dato = rand()%(9) + 1;
		MPI_Send(&dato, 1, MPI_INT, root, 99, comm);
    }
}
