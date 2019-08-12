#include "assistFunctions.h"
#include "conflict.h"
#include "mutate.h"
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void slave(int N, int NUM_SETS, int* board1, int* board2, int** newBoards) {

  // Holds the 4 newly mutated sets
  //int **newBoards = (int **)malloc(NUM_SETS * sizeof(int)); 
  int (*crs)[N] = malloc(sizeof(int[2][N]));
  // crs[0] = (int *)malloc(N * sizeof(int));
  // crs[1] = (int *)malloc(N * sizeof(int));

  for (int i = 0; i < NUM_SETS; i++) {
      newBoards[i] = (int *)malloc(N * sizeof(int));
  }

  newBoards[0] = board1;
  newBoards[1] = board2;
  mutateMaxConflict(N, newBoards[2]);
  mutateMaxConflict(N, newBoards[3]);
  //crs = 
  crossoverRandomSplit(N, board1, board2, 0.15, newBoards);
  //newBoards[2] = crs[0];
  //newBoards[3] = crs[1];

  //return newBoards;
}

int main(int argc, char **argv)
{ 
    srand(time(NULL));
    int numSets = 4;
    int N = 8;
    double genDivFactor = 0.2;
    int iter = 0;
    int rank, numranks, killCode;

    int** boards = (int**)malloc(numSets*sizeof(int));
    int** newBoards = (int**)malloc(numSets*sizeof(int));

    for(int i = 0; i < numSets; i++) {
        boards[i] = (int*)malloc(N*sizeof(int));
        newBoards[i] = (int*)malloc(N*sizeof(int));
    }

    int* conflictScores = (int *)malloc(numSets * sizeof(int));
    int* newConflictScores = (int *)malloc(numSets * sizeof(int));

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0) {
        for (int i = 0; i < numSets; i++) {
            boards[i] = (int *)malloc(N * sizeof(int));
            newBoards[i] = (int *)malloc(N * sizeof(int));
            fillRandom(N, boards[i]);
            conflictScores[i] = computeConflictScore(N, boards[i]);
            printf("Board %d | Conflic Score: %d\n", i, conflictScores[i]);
            printBoard(N, boards[i]);
        }

        //int iter = 0;
        //if(rank == 0) {
        while (sum(numSets, conflictScores) != 0) {
            int i1 = rand() % numSets;
            int i2;
            do {
                i2 = rand() % numSets;
            } while(i2 == i1);


            for(int i = 1; i < numranks; i++) {
                killCode = 0;
                MPI_Send(&killCode, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                //MPI_Send(&i1, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                //MPI_Send(&i2, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                for(int j = 0; j < numSets; j++) {
                    MPI_Send(boards[j], N, MPI_INT, i, 0, MPI_COMM_WORLD);
                }
            }

            for(int i = 1; i < numranks; i++) {
                for(int j = 0; j < numSets; j++) {
                    MPI_Recv(newBoards[j], N, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
                //newBoards = slave(N, numSets, boards[i1], boards[i2]);
                for (int j = 0; j < numSets; j++) {
                    newConflictScores[j] = computeConflictScore(N, newBoards[j]);
                }

                for (int p = 0; p < numSets; p++) {
                    int maxExistingConflictScore = max(numSets, conflictScores);
                    if (newConflictScores[p] < maxExistingConflictScore) {
                        int j = getIndex(numSets, conflictScores, maxExistingConflictScore);
                        boards[j] = newBoards[p];
                        conflictScores[j] = newConflictScores[p];
                    }
                }

                if (randDouble() < genDivFactor) {
                    mutateTrulyRandom(N, boards[rand() % numSets]);
                }
            }
        

            if (iter % 100 == 0) {
                printf("Iter: %d  Conflict Scores: ", iter);
                for (int i = 0; i < numSets; i++) {
                    printf("%d ", conflictScores[i]);
                }
                printf("\n");
            }

            iter++;
        }
    } else {
        while(1 == 1) {
            MPI_Recv(&killCode, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if(killCode == 1) {
                MPI_Finalize();
                return 0;
            }
            //printf("Entered Rank %d\n", rank);
            int i1 = rand() % numSets;
            int i2;
            do {
                i2 = rand() % numSets;
            } while(i2 == i1);
            //MPI_Recv(&i1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //MPI_Recv(&i2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for(int i = 0; i < numSets; i++) {
                MPI_Recv(boards[i], N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            //printf("At Rank: %d, Received Data\n", rank);
            slave(N, numSets, boards[i1], boards[i2], newBoards);
            for(int i = 0; i < numSets; i++) {
                MPI_Send(newBoards[i], numSets, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }

    }
    
    for (int i = 0; i < numSets; i++) {
        printf("Board %d | Conflict Score: %d\n", i, conflictScores[i]);
        printBoard(N, boards[i]);
    }
    killCode = 1;
    for(int i = 0; i < numranks; i++) {
        MPI_Send(&killCode, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }

    printf("Solved in %d iterations\n", iter);
}
