#include "mpi.h"
#include "assistFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

  // Number of chess boards
  num_sets = 4;
  // Size of the board
  N = 8;
  gen_div_factor = 0.2;

  srand(time(0));

  // Allocates all the sets into a 2D array
  // [[Set 1: N * N], [Set 2: N * N], ..., [Set num_sets: N * N]]
  int(*sets)[N * N] = malloc(sizeof(int[num_sets][N * N]));

  // Sets the initial value of the sets
  for (int p = 0; p < num_sets; p++)
  {
    for (int i = 0; i < N * N; ++i)
    {
      sets[p][i] = rand() % N;
    }
  }

  // Prints the sets
  for (int p = 0; p < num_sets; p++)
  {
    printBoard(N, sets[p]);
  }

  // Allocate conflict score array
  int *conflict_scores = (int *)malloc(num_sets * sizeof(int));

  // Populates conflict score array
  for (int i = 0; i < num_sets; i++)
  {
    conflict_scores[i] = computeConflictScore(N, sets[i]);
  }

  // // Prints conflict scores
  for (int i = 0; i < num_sets; i++)
  {
    printf("%d ", conflict_scores[i]);
  }
  printf("\n");

  // While the sum of the conflict scores isn't 0
  // Perform mutate_max_conflict and crossover_random_split on the sets

  // If the conflict scores of the new sets is lower than the existing sets
  // Replace the set with the highest conflict score with the corresponding         lower set
  // end if

  // end while

  MPI_Finalize();
  return 0;
}
