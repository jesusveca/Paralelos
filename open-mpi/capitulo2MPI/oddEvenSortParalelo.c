#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define N 100

void init(int* data, int rank) {
  int i;
  srand(rank);
  for (i = 0; i < N; i++)   data[i] = rand( ) % 1000;
}

void print(int* data, int rank) {
  int i;
  printf("Proceso %d: ");
  for (i = 0; i < N; i++)  printf("%d ", data[i]);
  printf("\n");
}

int cmp(const void* ap, const void* bp) {
  int a = * ((const int*) ap);  int b = * ((const int*) bp);
  if (a < b)        return -1;
  else if (a > b)   return 1;
  else              return 0;
}

int max_index(int* data) { // Encontrar el índice del elemento más grande en una matriz
  int i, max = data[0], maxi = 0;
  for (i = 1; i < N; i++)
    if (data[i] > max) {
      max = data[i];
      maxi = i;
  }
  return maxi;
}

int min_index(int* data) { // Encontrar el índice del elemento más pequeño en una matriz
  int i, min = data[0], mini = 0;
  for (i = 1; i < N; i++) {
    if (data[i] < min) {
      min = data[i];
      mini = i;
    }
  }
  return mini;
}

void parallel_sort(int* data, int rank, int size) { // algoritmo ODD-EVEN SORT
  int i;
  int other[N]; // La matriz que usamos para leer de un partner

  for (i = 0; i < size; i++) { //Necesitamos aplicar fases P donde P es el número de procesos
    qsort(data, N, sizeof(int), &cmp); // ordenando nuestro array local
    int partener; // encontrar nuestro partner en esta fase
    if (i % 2 == 0) { // si es una fase par
      if (rank % 2 == 0) partener = rank + 1;// si no estamos en un proceso par
      else partener = rank - 1;
    } 
    else {
      if (rank % 2 == 0) partener = rank - 1; // esta es una fase impar, hacer lo opuesto
      else partener = rank + 1;
    }
    if (partener < 0 || partener >= size)  continue; // si el parter es invalido, debemos ir a la sgte iteracion

    if (rank % 2 == 0) { // hacer el swap proceso par envia primero y proceso impart recive primero
      MPI_Send(data, N, MPI_INT, partener, 0, MPI_COMM_WORLD);
      MPI_Recv(other, N, MPI_INT, partener, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } 
    else {
      MPI_Recv(other, N, MPI_INT, partener, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(data, N, MPI_INT, partener, 0, MPI_COMM_WORLD);
    }
    if (rank < partener) { // hacer el merge de datos si queremos mas grandes o pequenos
      while (1) { // mantenemos  keys pequenas
        int mini = min_index(other); // encontrar el mas pequeno en el otro array.
        int maxi = max_index(data); // encontrar el mas grande en el array 
        if (other[mini] < data[maxi]) { // si el pequeno esta en el otro array es menor que el mas grande en el nuestro, hacemos swap;
          int temp = other[mini];
          other[mini] = data[maxi];
          data[maxi] = temp;
        }
        else break ;// paramos porque el mas pequeno esta ahora en los datos;
    }
    } 
    else {
      while (1) { // mantenemos keys grandes
        int maxi = max_index(other); // encontramos el mas grande en otro array;
        int mini = min_index(data); // encontramos el mas pequeno en otro array
        if (other[maxi] > data[mini]) { // si el mas grande esta en otro array mas grande que el pequeno en el nuestro, intercambiamos
          int temp = other[maxi];
          other[maxi] = data[mini];
          data[mini] = temp;
        } 
        else break ;// paramos porque el mas pequeno esta ahora en los datos;
      }
    }
  }
}
int main(int argc, char const *argv[]) {
  /* our rank and size */
  int rank, size;
  double MPI_Wtime(void);
  double start,finish;
  start=MPI_Wtime(); 	
  int data[N];

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  init(data, rank);

  parallel_sort(data, rank, size);
  finish=MPI_Wtime();
  printf("proc %d > Elapsed time = %e seconds\n",rank,(finish-start));
  MPI_Finalize( );
  return 0;
}