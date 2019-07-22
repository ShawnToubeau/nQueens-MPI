#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char **argv)
{

  // Initial variables
  int rank, totalRanks, num_sets, N;
  double gen_div_factor;
  // Initialize MPI
  MPI_Init(&argc, &argv);
  // Get total number of ranks
  MPI_Comm_size(MPI_COMM_WORLD, &totalRanks);
  // Get current rank
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Generate the sets

  // Compute the conflict scores

  // While the sum of the conflict scores isn't 0
  // Perform mutate_max_conflict and crossover_random_split on the sets

  // If the conflict scores of the new sets is lower than the existing sets
  // Replace the set with the highest conflict score with the corresponding         lower set
  // end if

  // end while

  MPI_Finalize();
  return 0;
}
