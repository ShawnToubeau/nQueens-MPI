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

  // Master communicator
  if (rank == 0) {
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

    // For testing purposes, so we do accidentally fall into a infinite loop
    int limit = 5;
    // While there are still conflicts, keep trying to get new boards
    while (sum(conflict_scores, num_sets) > 0 && limit > 0) {
      // Send the sets to all the slaves
      for (int nextRank = 1; nextRank < totalRanks; nextRank++) {
        MPI_Send(sets, num_sets * N * N, MPI_INT, nextRank, 0, MPI_COMM_WORLD);

        // Slaves do work to make new boards

        // TODO: make variable to store new sets sent from slaves
        // If we have 3 ranks, 1 of them is the master..
        // new_sets: [[slave-set-1],[slave-set-2]
        MPI_Recv(new_sets, num_sets * N * N, MPI_INT, nextRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }

      // With the new pool of sets from the slaves, iterate over the original sets and check to see if the corresponding sets are better
      for (int i = 0; i < num_sets, i++) {
        for (int j = 0; j < totalRanks -1; j++) {
          // If set[i] is worse than new_sets[j][i]
            // Replace set[i] with new_sets[i][j]
        }
      }

      // Recheck conflict scores
      for (int i = 0; i < num_sets; i++)
      {
        conflict_scores[i] = computeConflictScore(N, sets[i]);
      }
      limit--;
    }

    // // Prints conflict scores
    // for (int i = 0; i < num_sets; i++)
    // {
    //   printf("%d ", conflict_scores[i]);
    // }
    // printf("\n");
  }
  // Slave block 
  else 
  {
    // Allocates space for incoming sets
    int(*slave_sets)[N * N] = malloc(sizeof(int[num_sets][N * N]));

    MPI_Recv(slave_sets, num_sets * N * N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // printf("Rank: %d\n", rank);
    // for (int p = 0; p < num_sets; p++)
    // {
    //   printBoard(N, slave_sets[p]);
    // }


    // Have the slave pick 2 random boards from the set and use mutate_max_conflict
    // Use crossover_random_split on the remaining two

    // MPI_SEND, send the new boards back to the master

  }

  MPI_Finalize();
  return 0;
}
