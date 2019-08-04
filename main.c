#include "mpi.h"
#include "assistFunctions.h"
#include "conflict.h"
#include "mutate.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>


int** slave(int *set1, int *set2, int N, int NUM_SETS) {

  // Holds the 4 newly mutated sets
  int **new_sets = (int **)malloc(NUM_SETS * sizeof(int *));
  int **mmc_sets = (int **)malloc(2*sizeof(int*));

  for (int i = 0; i < NUM_SETS; i++) {
      new_sets[i] = (int *)malloc(N * sizeof(int));
  }

  // TODO: Incorporate the mutate functions
  // TODO: Use OMP to improve performance

  mmc_sets = crossoverRandomSplit(N, set1, set2, .75);
  new_sets[0] = mmc_sets[0];
  new_sets[1] = mmc_sets[1];
  mutateMaxConflict(N, set1);
  mutateMaxConflict(N, set2);
  new_sets[2] = set1;
  new_sets[3] = set2;

  return new_sets;
}

int main(int argc, char **argv)
{
  // Initial variables
  int rank, totalRanks, NUM_BOARDS, N;
  double gen_div_factor;
  // Initialize MPI
  MPI_Init(&argc, &argv);
  // Get total number of ranks
  MPI_Comm_size(MPI_COMM_WORLD, &totalRanks);
  // Get current rank
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Number of chess boards
  NUM_BOARDS = 4;
  // Size of the board
  N = 4;
  gen_div_factor = 0.75;

  srand(time(0));

  // Master communicator
  if (rank == 0) {
    // Allocates all the sets into a 2D array
    // [[Set 1: N], [Set 2: N], ..., [Set NUM_BOARDS: N]]
    int(*sets)[N] = malloc(sizeof(int[NUM_BOARDS][N]));

    // Sets the initial value of the sets
    for (int p = 0; p < NUM_BOARDS; p++)
    {
      for (int i = 0; i < N; ++i)
      {
        sets[p][i] = rand() % N;
      }
    }

    // Allocate conflict score array
    int *conflict_scores = (int *)malloc(NUM_BOARDS * sizeof(int));

    // Populates conflict score array
    for (int i = 0; i < NUM_BOARDS; i++)
    {
      conflict_scores[i] = computeConflictScore(N, sets[i]);
    }

    int loopNum = 0;
    int newMinConf = INT_MAX;
    // While there are still conflicts, keep trying to get new boards
    while (sum(NUM_BOARDS, conflict_scores) > 0) {
      if(loopNum%1000 == 0) {
        printf("RUNNING LOOP: %d ; CONFLICT SCORES: ", loopNum);
        for(int i = 0; i < NUM_BOARDS; i++) {
          printf("%d ", conflict_scores[i]);    
        }
        printf("\n");
      }
      loopNum++;

      // Variables for storing the new set and accompanying conflict score from slaves
      int* optimal_set = (int*)malloc(N * sizeof(int));

      // Communicate with all the slaves
      // currently only set up for 1?
      for (int nextRank = 1; nextRank < totalRanks; nextRank++) {
        // Send the current sets
        MPI_Send(sets, NUM_BOARDS * N, MPI_INT, nextRank, 0, MPI_COMM_WORLD);
      }
      for (int nextRank = 1; nextRank < totalRanks; nextRank++) {
        // Wait to receive a improved set + it's conflict score
        MPI_Recv(optimal_set, N, MPI_INT, nextRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&newMinConf, 1, MPI_INT, nextRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      
        int currMaxConf = max(NUM_BOARDS, conflict_scores);
        // Checks if the new conflict score is better than the existing worse score
        if (newMinConf < currMaxConf) {
          int currMaxConflictIndex = getIndex(NUM_BOARDS, conflict_scores, currMaxConf);
          

          // Replace the board
          for (int i = 0; i < N; i++) {
            sets[currMaxConflictIndex][i] = optimal_set[i];
          }
          if(randDouble() < gen_div_factor) {
              mutateTrulyRandom(N, sets[rand() % N]);
          }

          // Update the conflict score
          conflict_scores[currMaxConflictIndex] = newMinConf;
        }
      }
    } 

    printf("RUNNING LOOP: %d ; CONFLICT SCORES: ", loopNum);
    for(int i = 0; i < NUM_BOARDS; i++) {
      printf("%d ", conflict_scores[i]);    
    }
  }
  // Slave block
  else
  {
    while(1 == 1) {
      // Allocates space for incoming sets from master
      int(*incoming_sets)[N] = malloc(sizeof(int[NUM_BOARDS][N]));
      MPI_Recv(incoming_sets, NUM_BOARDS * N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      // Creates 4 new boards for every slave() call
      int** new_sets_1 = slave(incoming_sets[0], incoming_sets[1], N, NUM_BOARDS);
      int** new_sets_2 = slave(incoming_sets[2], incoming_sets[3], N, NUM_BOARDS);

      int *new_conflict_scores = (int *)malloc(NUM_BOARDS * 2 * sizeof(int));

      // Populates new conflict score array
      for (int i = 0; i < NUM_BOARDS * 2; i++)
      {
        if (i < 4) {
          new_conflict_scores[i] = computeConflictScore(N, new_sets_1[i]);
        } else {
          new_conflict_scores[i] = computeConflictScore(N, new_sets_2[i - NUM_BOARDS]);
        }
      }

      int minConflict = min(NUM_BOARDS * 2, new_conflict_scores);
      int minConflictIndex = getIndex(NUM_BOARDS * 2, new_conflict_scores, minConflict);

      //printf("RANK: %d, CONFLICT SCORE: %d\n", rank, minConflict);


      // Sends the board with the lowest conflict score back to master
      if (minConflictIndex < 4) {
        MPI_Send(new_sets_1[minConflictIndex], N, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&minConflict, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      } else {
        MPI_Send(new_sets_2[minConflictIndex - NUM_BOARDS], N, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&minConflict, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      }
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
  return 0;
}
