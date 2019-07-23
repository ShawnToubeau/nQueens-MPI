#include <stdio.h>
#include <stdlib.h>
// #include "mpi.h"

int main(int argc, char **argv)
{

  // Initial variables
  int rank, totalRanks, num_sets, N;
  double gen_div_factor;
  // Initialize MPI
  // MPI_Init(&argc, &argv);
  // Get total number of ranks
  // MPI_Comm_size(MPI_COMM_WORLD, &totalRanks);
  // Get current rank
  // MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  num_sets = 4;
  N = 8;
  gen_div_factor = 0.2;

  // Allocates all the sets into a 3D array
  int(*sets)[N][N] = malloc(sizeof(int[num_sets][N][N]));

  // Sets the initial value
  for (size_t p = 0; p < num_sets; p++)
  {
    for (size_t i = 0; i < N; ++i)
    {
      for (size_t j = 0; j < N; ++j)
      {
        sets[p][i][j] = p;
      }
    }
  }

  for (size_t p = 0; p < num_sets; p++)
  {
    printf("Outter set %lu\n", p);
    for (size_t i = 0; i < N; ++i)
    {
      for (size_t j = 0; j < N; ++j)
      {
        printf(" %d", sets[p][i][j]);
      }
      printf("\n");
    }
  }

  // Generate the sets

  // Compute the conflict scores

  // While the sum of the conflict scores isn't 0
  // Perform mutate_max_conflict and crossover_random_split on the sets

  // If the conflict scores of the new sets is lower than the existing sets
  // Replace the set with the highest conflict score with the corresponding         lower set
  // end if

  // end while

  // MPI_Finalize();
  return 0;
}
