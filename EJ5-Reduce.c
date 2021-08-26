#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>

void Reduce(int mytid, int nproc,int operacion, int root, MPI_Comm comm);

void main(int argc, char  **argv){
	int mytid, nproc;
    srand (time(NULL));
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &mytid);
    Reduce(mytid, nproc,atoi(argv[1]),atoi(argv[2]),MPI_COMM_WORLD);
    MPI_Finalize();
}

void Reduce(int mytid, int nproc,int operacion, int root, MPI_Comm comm){
	int i;
    MPI_Status status;
    if (mytid==root){
        int resultado,datoRoot=rand()%(9) + 1;
        int *datorec = (int*)malloc(sizeof(int)*nproc);
        for (i = 0; i < nproc; ++i){
            if(i!=root){
                MPI_Recv(&datorec[i], 1, MPI_INT, i, 99, comm, &status);
            }
            else{
                datorec[i]=datoRoot;
            }
        }
        resultado=datorec[0];
        for (i = 1; i < nproc; ++i){
            switch(operacion){
                case 0: 
                        resultado+=datorec[i];
                        break;
                case 1: resultado-=datorec[i];
                        break;
                case 2: resultado*=datorec[i];
                        break;
            }
        }
        printf("Se envio %d \n", datoRoot);
        printf("El resultado es: %d\n", resultado);
        free(datorec);
    }
    else{
        srand (time(NULL)); 
        int dato = rand()%(9) + 1;
        printf("Se envio %d \n",dato );
        MPI_Send(&dato, 1, MPI_INT, root, 99, comm);
    }
}