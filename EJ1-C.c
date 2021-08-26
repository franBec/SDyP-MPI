#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<mpi.h>
#define BUFF 21

void master(int nproc, MPI_Comm comm);
void slave(int mytid, MPI_Comm comm);

void main(int argc, char **argv){
    int mytid, nproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &mytid);
    if(mytid == 0)
        master(nproc,MPI_COMM_WORLD);
    else
        slave(mytid,MPI_COMM_WORLD);
    MPI_Finalize();
}

void master(int nproc, MPI_Comm comm){
    char* recvbuf = (char*)malloc(sizeof(char)*BUFF);  
    MPI_Status status;
    int i;
    int success;
    printf("Hola, soy el proceso 0, hay %d procesos, y recibo:\n", nproc);
    for(i=1;i<nproc;i++){
        success = MPI_Recv( recvbuf, BUFF, MPI_CHAR, i, 99, comm, &status);
        if(success==MPI_SUCCESS){
            printf("%s\n", recvbuf);
        }
        else{
            printf("Error al recibir mensaje de proceso %d\n",i);
        }
    }
    free((void*) recvbuf);
}

void slave(int mytid, MPI_Comm comm){
    char* str1 = "Hola del proceso ";
    char* sendbuf =(char*)malloc(sizeof(char)*BUFF);
    char *num;
    asprintf(&num, "%d", mytid);
    strcat(strcpy(sendbuf, str1), num);
    MPI_Send(sendbuf, BUFF, MPI_CHAR, 0, 99, comm);
    free((void*) str1);
    free((void*) sendbuf);
    free((void*) num);
}