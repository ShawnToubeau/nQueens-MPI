#include "mpi.h"
#include "assistFunctions.h"
#include "conflict.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int** slave(int *set1, int *set2, int N, int num_sets) {

  // Array that will hold the 4 newly mutated sets
  int **new_sets = (int **)malloc(num_sets * sizeof(int *)); 
  
  for (int row = 0; row < N; row++) {
      new_sets[row] = (int *)malloc(N * N * sizeof(int));
  }

  // TODO: Incorporate the mutate functions


  // Simulates the return of the mutated sets
  // [[set1], [set1], [set2], [set2]]
  printf("Slave\n");
  for (int i = 0; i < num_sets; i++) {
    for (int j = 0; j < N * N; j++) {
      if (i < 2) {
        new_sets[i][j] = set1[j];
      } else { 
        new_sets[i][j] = set2[j];
      }
      
    }
  }

  for (int i = 0; i < num_sets; i++) {
    for (int j = 0; j < N * N; j++) {
      if (j % N == 0) {
        printf("\n");
      }
      printf(" %d", new_sets[i][j]);
      
    }
    printf("\n");
  }
  printf("\n");

  return new_sets; 
}

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

    // NOTE: For testing purposes so we do not accidentally fall into a infinite loop
    int limit = 1;
    // While there are still conflicts, keep trying to get new boards
    while (sum(num_sets, conflict_scores) > 0 && limit > 0) {
      // Send the sets to all the slaves
      for (int nextRank = 1; nextRank < totalRanks; nextRank++) {
        MPI_Send(sets, num_sets * N * N, MPI_INT, nextRank, 0, MPI_COMM_WORLD);

        // Slaves do work to make new boards

        // TODO: make an array to store mutated sets sent from slaves

        // MPI_Recv(new_sets, num_sets * N * N, MPI_INT, nextRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }

      // With the new pool of mutated sets from the slaves, iterate over the original sets and check to see if the corresponding sets are better
      for (int i = 0; i < num_sets; i++) {
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
  }
  // Slave block 
  else 
  {
    // Allocates space for incoming sets from master
    int(*incoming_sets)[N * N] = malloc(sizeof(int[num_sets][N * N]));

    MPI_Recv(incoming_sets, num_sets * N * N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // HELP: not sure if all the stuff below it the optimal way of handling things
    // because slave() returns pointers to the mutated sets
    // Allocate space for the new sets to be returned from slave()
    // [[4 sets], [4 sets]]
    int(*new_sets)[N * N] = malloc(2 * sizeof(int[num_sets][N * N]));

    int** new_sets_1 = slave(incoming_sets[0], incoming_sets[1], N, num_sets);
    int** new_sets_2 = slave(incoming_sets[2], incoming_sets[3], N, num_sets);

    printf("Mutated Sets\n");

    for (int i = 0; i < num_sets * 2; i++) {
      for (int j = 0; j < N * N; j++) {
        if (i < 4) {
          new_sets[i][j] = new_sets_1[i][j];
        } else {
          new_sets[i][j] = new_sets_2[i - num_sets][j];
        }
      }
    }


    for (int i = 0; i < num_sets * 2; i++) {
      for (int j = 0; j < N * N; j++) {
        if (j % N == 0) {
          printf("\n");
        }
        if (j % (N * N) == 0) {
          printf("\n");
        }
        printf(" %d", new_sets[i][j]);
        
      }
      printf("\n");
    }

    // MPI_SEND, send the mutated boards back to the master
  }

  MPI_Finalize();
  return 0;
}
