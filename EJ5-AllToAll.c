#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>

void AllToAll(int mytid, int nproc,MPI_Comm comm);

void main(int argc, char  **argv){
	int mytid, nproc;
    srand (time(NULL));
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &mytid);
    AllToAll(mytid, nproc,MPI_COMM_WORLD);
    MPI_Finalize();
}

void AllToAll(int mytid, int nproc, MPI_Comm comm){
	int i,j;
    int *valoresPropios = (int*)malloc(sizeof(int)*nproc);
	int *valoresResultantes = (int*)malloc(sizeof(int)*nproc);
    MPI_Status status;
    //Aca inicio con valores de prueba para 5 procesos
    /*
    switch(mytid){
        case 0: valoresPropios[0]=2;
                valoresPropios[1]=2;
                valoresPropios[2]=2;
                valoresPropios[3]=2;
                valoresPropios[4]=2;
                break;
        case 1: valoresPropios[0]=3;
                valoresPropios[1]=3;
                valoresPropios[2]=3;
                valoresPropios[3]=3;
                valoresPropios[4]=3;
                break;
        case 2: valoresPropios[0]=4;
                valoresPropios[1]=4;
                valoresPropios[2]=4;
                valoresPropios[3]=4;
                valoresPropios[4]=4;
                break;
        case 3: valoresPropios[0]=5;
                valoresPropios[1]=5;
                valoresPropios[2]=5;
                valoresPropios[3]=5;
                valoresPropios[4]=5;
                break;
        case 4: valoresPropios[0]=6;
                valoresPropios[1]=6;
                valoresPropios[2]=6;
                valoresPropios[3]=6;
                valoresPropios[4]=6;
                break;
    }
    */
    for (i = 0; i < nproc; ++i){
        if (i==mytid){
            for (j = 0; j < nproc; ++j){
                if(mytid!=j)
                    MPI_Send(&valoresPropios[i], 1, MPI_INT, j, 99, comm);
            }
            valoresResultantes[mytid]=valoresPropios[mytid];
        }
        else{
            MPI_Recv(&valoresResultantes[i], 1, MPI_INT, i, 99, comm, &status);
        }       
    }
    free(valoresPropios);
    free(valoresResultantes);
}