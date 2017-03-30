#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  const int limit = 10;
  MPI_Init(NULL, NULL);
  int rank; //obtener ID del proceso
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size; //numero de procesos
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int contadorPingPong = 0;
  int rankPart = (rank + 1) % 2;
  while (contadorPingPong < limit) {
    if (rank == contadorPingPong % 2) { 
      contadorPingPong++;
      MPI_Send(&contadorPingPong, 1, MPI_INT, rankPart, 0, MPI_COMM_WORLD);
      printf("%d envia a destino  :%d   con contador:%d \n", rank, rankPart, contadorPingPong);
    } else {
      MPI_Recv(&contadorPingPong, 1, MPI_INT, rankPart, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%d recibe de origen :%d   con contador:%d \n", rank, rankPart, contadorPingPong);
    }
  }
  MPI_Finalize();
}