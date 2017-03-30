#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  const int limit = 10;
  MPI_Init(NULL, NULL);
  int idProcess; //obtener ID del proceso
  MPI_Comm_rank(MPI_COMM_WORLD, &idProcess);
  int quantity; //numero de procesos
  MPI_Comm_size(MPI_COMM_WORLD, &quantity);

  int cont = 0;
  int enemy = (idProcess + 1) % 2;
  while (cont < limit) {
    if (idProcess == cont % 2) { 
      cont++;
      MPI_Send(&cont, 1, MPI_INT, enemy, 0, MPI_COMM_WORLD);
      printf("%d envia a destino  :%d   con contador:%d \n", idProcess, enemy, contador);
    } else {
      MPI_Recv(&cont, 1, MPI_INT, enemy, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%d recibe de origen :%d   con contador:%d \n", idProcess, enemy, contador);
    }
  }
  MPI_Finalize();
}