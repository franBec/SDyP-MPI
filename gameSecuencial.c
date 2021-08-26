#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int** newMatrix(int filas, int columnas){
  int** arr = (int**)malloc(sizeof(int*) * filas + sizeof(int) * columnas * filas);
  int* ptr = (int *)(arr + filas);

  for(int i = 0; i < filas; i++)
      arr[i] = (ptr + columnas * i);

  return arr;
}

int randInRange(int min, int max){
  return min + (int) (rand() / (double) (RAND_MAX + 1) * (max - min + 1));
}

void printMatrix(int* matrix[], int filas, int columnas){
  for (int i = 0; i <  filas; i++){
    for (int j = 0; j < columnas; j++)
        printf(matrix[i][j] ? " # " : " O ");
    printf("\n");
  }
  printf("\n");
}

void inicializarMatrixRANDOM(int* matrix[], int filas, int columnas, int valorMinimo, int valorMaximo){
  for (int i = 0; i <  filas; i++)
    for (int j = 0; j < columnas; j++)
       matrix[i][j] = randInRange(valorMinimo, valorMaximo);
}

int** getVecindario(int* matrix[], int filas, int columnas, int posX, int posY){
  int** vecindario = newMatrix(3,3);
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      vecindario[i][j] = matrix[(filas+posY-1+i)%filas][(columnas+posX-1+j)%columnas]; //matriz circular

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

void main(){
  srand(time(NULL));
  int filas = 8, columnas = 8; //DIMENSION DEL JUEGO
  int generacion = 0;

  int** matrix = newMatrix(filas, columnas);
  inicializarMatrixRANDOM(matrix, filas, columnas, 0, 1);

  int** matrixProxima = newMatrix(filas, columnas);

  while(1){
    printf("GENERACION: %d\n\n", generacion);
    printMatrix(matrix,filas,columnas);
    for (int i = 0; i < filas; i++)
      for (int j = 0; j < columnas; j++)
        matrixProxima[i][j] = nextState(matrix, filas, columnas, i, j);

    matrix = matrixProxima;
    generacion++;
    printf("\n");
    system("pause");
    printf("\n");
  }
}
