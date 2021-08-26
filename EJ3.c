#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mpi.h>
#include<time.h>

void matrizxVector(int mytid,int nproc,MPI_Comm comm);


void main(int argc, char **argv){
    int mytid, nproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &mytid);
    matrizxVector(mytid,nproc,MPI_COMM_WORLD);
    MPI_Finalize();
}


void matrizxVector(int mytid,int nproc, MPI_Comm comm){
    int A[nproc][nproc];
    int x[nproc], b[nproc], fila[nproc];
    srand (time(NULL)); 
    if(mytid==0){
        printf("La matriz y el vector generados son:\n");
        for (int i = 0; i<nproc; i++) {
            for (int j = 0; j<nproc; j++) {
                if (j == 0){
                    printf("[");
                }
                A[i][j] = rand()%(9) + 1;
                printf(" %d ", A[i][j]);
                if (j == nproc - 1){
                    printf("]");
                }
            }
            x[i] = rand()%(9) + 1;
            printf("\t[%d]\n",x[i]);
        }
        printf("\n");
    }
    MPI_Scatter(A, nproc, MPI_INT, fila, nproc, MPI_INT, 0, comm);
    MPI_Bcast(x, nproc, MPI_INT, 0, comm);
    int resParcial = 0;
    for (int i = 0; i < nproc; i++) {
        resParcial += fila[i] * x[i];
    }
    MPI_Gather(&resParcial, 1, MPI_INT, b, 1, MPI_INT, 0, comm);
    if(mytid==0){
        printf("El resultado es:\n");
        for (int i = 0; i < nproc; i++){
            printf("\t[%d]\n",b[i]);
        }
    }
}