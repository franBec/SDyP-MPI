#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<mpi.h>
#define BUFF 5

void ring(int mytid,int nproc, int rootf,int loop,MPI_Comm comm);

//ARGUMENTOS -np [numero de procesos] [cantidad de giros] [inicio del anillo]

void main(int argc, char **argv){
    int mytid, nproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &mytid);
    ring(mytid,nproc,atoi(argv[2]),atoi(argv[1]),MPI_COMM_WORLD);
    MPI_Finalize();
}


void ring(int mytid,int nproc, int rootf, int loop, MPI_Comm comm){
    char* sendbuf =(char*)malloc(sizeof(char)*BUFF);
    char* recvbuf = (char*)malloc(sizeof(char)*BUFF);
    MPI_Status status;
    int i, recid;
    if(mytid==0){
        recid=nproc-1;
    }
    else{
        recid=mytid-1;
    }
    for(i=0;i<loop;i++){
        if(mytid==rootf){
            if(i==0){
                strcpy(sendbuf,"Hola");//ESTE ES EL MENSAJE QUE DEBE CIRCULAR
            }
            MPI_Send(sendbuf, BUFF, MPI_CHAR, (mytid+1)%nproc, 99, comm);
            printf("El proceso %d envio el M%d con el dato “%s” al proceso %d\n",mytid,(i*nproc)+mytid, sendbuf,(mytid+1)%nproc);
            MPI_Recv( recvbuf, BUFF, MPI_CHAR, recid, 99, comm, &status);
            strcpy(sendbuf,recvbuf);
        }
        else{
            MPI_Recv( recvbuf, BUFF, MPI_CHAR, recid, 99, comm, &status);
            strcpy(sendbuf,recvbuf);
            MPI_Send(sendbuf, BUFF, MPI_CHAR, (mytid+1)%nproc, 99, comm);
            printf("El proceso %d envio el M%d con el dato “%s” al proceso %d\n",mytid,(i*nproc)+mytid, sendbuf, (mytid+1)%nproc);
        }
    }
    free((void*) recvbuf);
    free((void*) sendbuf);
}

