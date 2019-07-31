#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "assistFunctions.c"
#include "mpi.h"

// Calculates the conflict scores of a chess board
// int computeConflictScores(int N, int board[N][N])
// {
//   // This is temp
//   for (size_t i = 0; i < N; ++i)
//   {
//     for (size_t j = 0; j < N; ++j)
//     {
//       printf(" %d", board[i][j]);
//     }
//     printf("\n");
//   }
//   return 1;
// }

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

  // Allocates all the sets into a 3D array
  // [[Set 1: N * N], [Set 2: N * N], ..., [Set num_sets: N * N]]
  int(*sets)[N][N] = malloc(sizeof(int[num_sets][N][N]));

  // Sets the initial value of the sets
  for (size_t p = 0; p < num_sets; p++)
  {
    for (size_t i = 0; i < N; ++i)
    {
      for (size_t j = 0; j < N; ++j)
      {
        sets[p][i][j] = rand() % N;
      }
    }
  }

  // Prints the sets
  // for (size_t p = 0; p < num_sets; p++)
  // {
  //   printf("Outter set %lu\n", p);
  //   for (size_t i = 0; i < N; ++i)
  //   {
  //     for (size_t j = 0; j < N; ++j)
  //     {
  //       printf(" %d", sets[p][i][j]);
  //     }
  //     printf("\n");
  //   }
  // }

  // Allocate conflict score array
  int *conflict_scores = (int *)malloc(num_sets * sizeof(int));

  // Populates conflict score array
  for (int i = 0; i < num_sets; i++)
  {
    conflict_scores[i] = computeConflictScores(N, N, sets[i]);
  }

  // Prints conflict scores
  for (int i = 0; i < num_sets; i++)
  {
    printf("%d ", conflict_scores[i]);
  }

  // While the sum of the conflict scores isn't 0
  // Perform mutate_max_conflict and crossover_random_split on the sets

  // If the conflict scores of the new sets is lower than the existing sets
  // Replace the set with the highest conflict score with the corresponding         lower set
  // end if

  // end while

  MPI_Finalize();
  return 0;
}
