#include "mpi.h"
#include "assistFunctions.h"
#include "conflict.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int** slave(int *set1, int *set2, int N, int NUM_SETS) {

  // Holds the 4 newly mutated sets
  int **new_sets = (int **)malloc(NUM_SETS * sizeof(int *)); 
  
  for (int i = 0; i < NUM_SETS; i++) {
      new_sets[i] = (int *)malloc(N * N * sizeof(int));
  }

  // TODO: Incorporate the mutate functions

  return new_sets; 
}

int main(int argc, char **argv)
{
  // Initial variables
  int rank, totalRanks, NUM_SETS, N;
  double gen_div_factor;
  // Initialize MPI
  MPI_Init(&argc, &argv);
  // Get total number of ranks
  MPI_Comm_size(MPI_COMM_WORLD, &totalRanks);
  // Get current rank
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Number of chess boards
  NUM_SETS = 4;
  // Size of the board
  N = 8;
  gen_div_factor = 0.2;

  srand(time(0));

  // Master communicator
  if (rank == 0) {
    // Allocates all the sets into a 2D array
    // [[Set 1: N * N], [Set 2: N * N], ..., [Set NUM_SETS: N * N]]
    int(*sets)[N * N] = malloc(sizeof(int[NUM_SETS][N * N]));

    // Sets the initial value of the sets
    for (int p = 0; p < NUM_SETS; p++)
    {
      for (int i = 0; i < N * N; ++i)
      {
        sets[p][i] = rand() % N;
      }
    }

    // Allocate conflict score array
    int *conflict_scores = (int *)malloc(NUM_SETS * sizeof(int));

    // Populates conflict score array
    for (int i = 0; i < NUM_SETS; i++)
    {
      conflict_scores[i] = computeConflictScore(N, sets[i]);
    }

    // While there are still conflicts, keep trying to get new boards
    while (sum(NUM_SETS, conflict_scores) > 0) {
      // Variables for storing the new set and accompanying conflict score from slaves
      int* optimal_set = (int*)malloc(N * N * sizeof(int));
      int newMinConf;

      // Communicate with all the slaves
      for (int nextRank = 1; nextRank < totalRanks; nextRank++) {
        // Send the current sets
        MPI_Send(sets, NUM_SETS * N * N, MPI_INT, nextRank, 0, MPI_COMM_WORLD);
        // Wait to receive a improved set + it's conflict score
        MPI_Recv(optimal_set, N * N, MPI_INT, nextRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&newMinConf, 1, MPI_INT, nextRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }

      printf("Current Sets\n");
      for (int p = 0; p < NUM_SETS; p++)
      {
        printBoard(N, sets[p]);
      }

      printf("Current Conflict Scores\n");
      for (int i = 0; i < NUM_SETS; i++) {
        printf("%d ", conflict_scores[i]);
      }
      printf("\n\n");

      int currMaxConf = max(NUM_SETS, conflict_scores);
      // Checks if the new conflict score is better than the existing worse score
      if (newMinConf < currMaxConf) {
        int currMaxConflictIndex = getIndex(NUM_SETS, conflict_scores, currMaxConf);
        // Replace the board
        for (int i = 0; i < N * N; i++) {
          sets[currMaxConflictIndex][i] = optimal_set[i];
        }
        // Update the conflict score
        conflict_scores[currMaxConflictIndex] = newMinConf;
      }

      printf("New Conflict Scores\n");
      for (int i = 0; i < NUM_SETS; i++) {
        printf("%d ", conflict_scores[i]);
      }
      printf("\n\n");
      
      // Prints the sets
      printf("New Sets\n");
      for (int p = 0; p < NUM_SETS; p++)
      {
        printBoard(N, sets[p]);
      }
    }
  }
  // Slave block 
  else 
  {
    // Allocates space for incoming sets from master
    int(*incoming_sets)[N * N] = malloc(sizeof(int[NUM_SETS][N * N]));
    MPI_Recv(incoming_sets, NUM_SETS * N * N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Creates 4 new boards for every slave() call
    int** new_sets_1 = slave(incoming_sets[0], incoming_sets[1], N, NUM_SETS);
    int** new_sets_2 = slave(incoming_sets[2], incoming_sets[3], N, NUM_SETS);

    int *new_conflict_scores = (int *)malloc(NUM_SETS * 2 * sizeof(int));

    // Populates new conflict score array
    for (int i = 0; i < NUM_SETS * 2; i++)
    {
      if (i < 4) {
        new_conflict_scores[i] = computeConflictScore(N, new_sets_1[i]);
      } else {
        new_conflict_scores[i] = computeConflictScore(N, new_sets_2[i - NUM_SETS]);
      }
    }

    int minConflict = min(NUM_SETS * 2, new_conflict_scores);
    int minConflictIndex = getIndex(NUM_SETS * 2, new_conflict_scores, minConflict);

    // Sends the board with the lowest conflict score back to master
    if (minConflictIndex < 4) {
      MPI_Send(new_sets_1[minConflictIndex], N * N, MPI_INT, 0, 0, MPI_COMM_WORLD);
      MPI_Send(&minConflict, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
      MPI_Send(new_sets_2[minConflictIndex - NUM_SETS], N * N, MPI_INT, 0, 0, MPI_COMM_WORLD);
      MPI_Send(&minConflict, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  }

  MPI_Finalize();
  return 0;
}
