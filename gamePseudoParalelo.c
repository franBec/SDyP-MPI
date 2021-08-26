//FUNCIONES AUXILIARES --casi idem a gameSecuencial.c
int** getVecindario(int* matrix[], int filas, int columnas, int posX, int posY){
  int** vecindario = newMatrix(3,3);
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      vecindario[i][j] = matrix[posY-1+i][posX-1+j]; //no circular

  return vecindario;
}

int cantidadVecinosVivos(int* matrix[]){
  int count = 0;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
        if(matrix[i][j] == 1)
          count++;

  if(matrix[1][1] == 1)
    count --;

  return count;
}

int nextState(int* matrix[], int filas, int columnas, int i, int j){
  int cantidadVecinos = cantidadVecinosVivos(getVecindario(matrix,filas,columnas,j,i));
  //if else
  if(matrix[i][j]==1){
    //estoy vivo, veo si debo morir
    if(cantidadVecinos <2 || cantidadVecinos>3)
      return 0;
    else
      return 1;
  }
  else{
    //estoy muerto, veo si debo vivir
    if(cantidadVecinos == 3)
      return 1;
    else
      return 0;
  }
}

void main{
	//argv[1] = int filas
	//argv[2] = int columnas

	int mytid, nproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &mytid);
    juegoDeLaVida(mytid,nproc,atoi(argv[1]),(argv[2]),MPI_COMM_WORLD);
    MPI_Finalize();
}


//PSEUDOCODIGO basado en
	//las filas se distribuyen en los procesos
	//los procesos se comunican entre ellos la información necesaria para continuar
	//la información se reune en master unicamente al momento de imprimir
void juegoDeLaVida(int mytid, int nproc, int filas, int columnas, MPI_Comm comm){
	
	int cociente = filas/nproc;
	int** matriz; //[filas+2] x [columnas+2]
	int** matrizReceptora; //[cociente + 2] x [columnas] 
	int** matrixProxima; //[cociente + 2] x [columnas]

	if(mytid == 0){

		matriz = newMatriz(filas+2,columnas+2); //+2 para que esté rodeada de ceros
		inicializarMatrixRANDOM(matriz, filas+2, columnas+2);
		printf("El juego arranca en este estado\n", );
		printMatriz(Matriz);
		
		//distribuyo la matriz es partes iguales entre mis procesos
		//asumo que la cantidad de procesos es multiplo de mi cantidad de filas
		for(int i = 1 ; i<nproc; i++){
			MPI_Send(matriz[i*cociente], (columnas+2)*(cociente+2), MPI_INT, i, 99, comm)
		}

		matrizReceptora = las primeras (cociente + 1) filas de matriz;

	else{
		//slaves hacen esto
		MPI_Recv(matrizReceptora[0], (columnas+2)*(cociente+2), MPI_INT, 0, 99, comm);
	}	

	//todos hacen esto
	for (int i = 0; i < cantidadTurnos; i++){
		for (int j = 1; j < columnas-1; j++){
			for (int k = 1; k <= cociente; k++){
				//nextState me retorna 0 o 1 de acuerdo a la cantidad de vecinos vivos
				matrixProxima[j][k] = nextState(matrizReceptora,j,k);
			}
		}
		matrizReceptora = matrixProxima;

		/*
		ENVIO DE INFORMACION PUNTO A PUNTO ENTRE PROCESOS
		si soy p0
			enviar columna cociente a p1
			recibir	de p1 la columna cociente+1

		si soy un pi
			recibir del anterior en la columna 0
			enviar columna 1 al anterior
			enviar columna cociente	al siguiente
			recibir del siguiente en la columna cociente+1

		si soy pn
			recibir del anterior en la columna 0
			enviar columna 1 al anterior
		*/
		
		//caso p0
		if(mytid==0){
			//enviar columna cociente a p1
			MPI_Send(matrizReceptora[cociente], (columnas+2), MPI_INT, 1, 99, comm);

			//recibir	de p1 la columna cociente+1
			MPI_Recv(matrizReceptora[cociente+1], (columnas+2), MPI_INT, 1, 99, comm);
		}

		//caso pi
		else if(mytid>0 && mytid<nproc-1){
			//recibir del anterior en la columna 0
			MPI_Recv(matrizReceptora[0], (columnas+2), MPI_INT, mytid-1, 99, comm);

			//enviar columna 1 al anterior
			MPI_Send(matrizReceptora[1], (columnas+2), MPI_INT, mytid-1, 99, comm);

			//enviar columna cociente al siguiente
			MPI_Send(matrizReceptora[cociente], (columnas+2), MPI_INT, mytid+1, 99, comm);

			//recibir del siguiente en la columna cociente+1
			MPI_Recv(matrizReceptora[cociente+1], (columnas+2), MPI_INT, mytid+1, 99, comm);
		}

		//caso pn
		else{
			//recibir del anterior en la columna 0
			MPI_Recv(matrizReceptora[0], (columnas+2), MPI_INT, mytid-1, 99, comm);

			//enviar columna 1 al anterior
			MPI_Send(matrizReceptora[1], (columnas+2), MPI_INT, mytid-1, 99, comm);			
		}

	}

	//ahora que terminaron los turnos solicitados, reuno toda la información en master
	MPI_Gather(matrizReceptora[1], cociente*(columnas+2), MPI_INT, matriz[0], cociente*(columnas+2), MPI_INT, 0, comm);

	//master imprime
	if(mytid==0){
		printMatriz(matriz); //print matriz imprime unicamente los lugares con información
	}
}