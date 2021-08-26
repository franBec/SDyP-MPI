#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<mpi.h>
#include<time.h>

void matrizxVector(int mytid,int nproc,int size,MPI_Comm comm);


void main(int argc, char **argv){
    int mytid, nproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &mytid);
    matrizxVector(mytid,nproc,atoi(argv[1]),MPI_COMM_WORLD);
    MPI_Finalize();
}


void matrizxVector(int mytid, int nproc,int size, MPI_Comm comm){
    int A[size][size];
    int x[size], b[size];
    MPI_Status status;
    srand (time(NULL)); 
    
    //SI SOY MASTER
        if(mytid==0){
            printf("La matriz y el vector generados son:\n");
            for (int i = 0; i<size; i++) {
                for (int j = 0; j<size; j++) {
                    if (j == 0){
                        printf("[");
                    }
                    A[i][j] = rand()%(9) + 1;
                    printf(" %d ", A[i][j]);
                    if (j == size - 1){
                        printf("]");
                    }
                }
                x[i] = rand()%(9) + 1;
                printf("\t[%d]\n",x[i]);
            }
            printf("\n");
        }

    //SI SOY MULTIPLO
        if(size%nproc==0){
            int coc = size/nproc;
            int matrizFilasAsignadas[coc][size];
            for (int i = 0; i < coc; i++){
                MPI_Scatter(A[i*nproc], size, MPI_INT, matrizFilasAsignadas[i], size, MPI_INT, 0, comm);
            }
            MPI_Bcast(x, size, MPI_INT, 0, comm);
            int resParcial[coc];
            for (int i = 0; i < coc; i++){
                resParcial[i]=0;
                for (int j = 0; j < size; j++) {
                    resParcial[i] += matrizFilasAsignadas[i][j] * x[j];
                }
            }
            for (int i = 0; i < coc; i++){
                MPI_Gather(&resParcial[i], 1, MPI_INT, &b[i*nproc], 1, MPI_INT, 0, comm);
            }
        }

        //SI NO SOY MULTIPLO
        else{
            int coc = size/nproc, mod = size%nproc;
            int matrizFilasAsignadas[coc+1][size];
            for (int i = 0; i < coc; i++){
                MPI_Scatter(A[i*nproc], size, MPI_INT, matrizFilasAsignadas[i], size, MPI_INT, 0, comm);
            }
            //Selecciono aquellos que tienen que trabajar extra
            if(mytid<mod){
                //Selecciono el proceso root
                if(mytid==0){
                    for (int i = 0; i < size; i++){
                        matrizFilasAsignadas[coc][i]=A[coc*nproc][i];
                    }
                    for (int i = 1; i <= mod; i++){
                        MPI_Send( A[(coc*nproc)+i], size, MPI_INT, i, 99, comm);
                    }
                }
                else{
                    MPI_Recv(matrizFilasAsignadas[coc], size, MPI_INT, 0, 99, comm, &status);
                }
            }

            //todos hacen esto
            MPI_Bcast(x, size, MPI_INT, 0, comm);
            int resParcial[coc];
            for (int i = 0; i < coc; i++){
                resParcial[i]=0;
                for (int j = 0; j < size; j++) {
                    resParcial[i] += matrizFilasAsignadas[i][j] * x[j];
                }
            }
            //si tengo que trabajar extra entro aca 
            if(mytid<mod){
                resParcial[coc]=0;
                for (int j = 0; j < size; j++) {
                    resParcial[coc] += matrizFilasAsignadas[coc][j] * x[j];
                }
            }
            //todos hacen esto
            for (int i = 0; i < coc; i++){
                MPI_Gather(&resParcial[i], 1, MPI_INT, &b[i*nproc], 1, MPI_INT, 0, comm);
            }
            //si tengo que hacer trabajo extra
            if(mytid<mod){
                if(mytid==0){
                    b[coc*nproc]=resParcial[coc];
                    printf("%d\n", mod);
                    for (int i = 1; i < mod; i++){
                        printf("Hola\n");
                        MPI_Recv(&b[(coc*nproc)+i], 1, MPI_INT, i, 99, comm, &status);
                    }
                }
                else{
                    MPI_Send(&resParcial[coc], 1, MPI_INT, 0, 99, comm);
                }
            }
        }

    //MASTER IMPRIME
    if(mytid==0){
        printf("El resultado es:\n");
        for (int i = 0; i < size; i++){
            printf("\t[%d]\n",b[i]);
        }
    }
}