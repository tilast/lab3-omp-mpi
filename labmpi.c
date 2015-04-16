#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define START 2
#define END   3
#define STEP  0.01

double y(double x);

int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // We are assuming at least 2 processes for this task
  if (world_size < 2) {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1); 
  }

  double start = 2.0;
  double end   = 3.0;
  double step  = 0.01;
  int source   = 0;
  double sampleSize    = (double)(end - start) / (double)(world_size - 1);
  int iterationsAmount = sampleSize / step;

  double partResult, result = 0;

  if (world_rank == 0) {
    for(int i = 1; i < world_size; ++i)
    {
      source = i;
      MPI_Recv(&partResult, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      result += partResult;
    }
    printf("Integral = %f\n", result);
  } else if (world_rank > 0) {
    partResult = 0;
    for(int i = 0; i < iterationsAmount; ++i) {
      partResult += y(START + sampleSize * (world_rank - 1) + i*step) * step;
    }

    printf("Part result = %f\n", partResult);
    MPI_Send(&partResult, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  }
  MPI_Finalize();
}

double y(double x)
{
  return pow(x, 3.0) / sqrt(pow(pow(x, 2.0) + 9.0, 3.0));
}