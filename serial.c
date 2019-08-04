#include "assistFunctions.h"
#include "conflict.h"
#include "mutate.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int** slave(int N, int NUM_SETS, int* board1, int* board2) {

  // Holds the 4 newly mutated sets
  int **newBoards = (int **)malloc(NUM_SETS * sizeof(int)); 
  int **crs = (int **)malloc(2 * N * sizeof(int));
  crs[0] = (int *)malloc(N * sizeof(int));
  crs[1] = (int *)malloc(N * sizeof(int));

  for (int i = 0; i < NUM_SETS; i++) {
      newBoards[i] = (int *)malloc(N * sizeof(int));
  }

  newBoards[0] = board1;
  newBoards[1] = board2;
  mutateMaxConflict(N, newBoards[0]);
  mutateMaxConflict(N, newBoards[1]);
  crs = crossoverRandomSplit(N, board1, board2, 0.15);
  newBoards[2] = crs[0];
  newBoards[3] = crs[1];

  return newBoards;
}

int main(int argc, char **argv)
{ 
    srand(time(NULL));
    int numSets = 4;
    int N = 8;
    double genDivFactor = 0.2;

    int** boards = (int **)malloc(numSets * N * sizeof(int));
    int** newBoards = (int **)malloc(numSets * N * sizeof(int));

    int* conflictScores = (int *)malloc(numSets * sizeof(int));
    int* newConflictScores = (int *)malloc(numSets * sizeof(int));

    for (int i = 0; i < numSets; i++) {
        boards[i] = (int *)malloc(N * sizeof(int));
        newBoards[i] = (int *)malloc(N * sizeof(int));
        fillRandom(N, boards[i]);
        conflictScores[i] = computeConflictScore(N, boards[i]);
        printf("Board %d | Conflic Score: %d\n", i, conflictScores[i]);
        printBoard(N, boards[i]);
    }

    int iter = 0;
    while (sum(numSets, conflictScores) != 0) {
        int i1 = rand() % numSets;
        int i2;
        do {
            i2 = rand() % numSets;
        } while(i2 == i1);

        newBoards = slave(N, numSets, boards[i1], boards[i2]);
        for (int i = 0; i < numSets; i++) {
            newConflictScores[i] = computeConflictScore(N, newBoards[i]);
        }

        for (int i = 0; i < numSets; i++) {
            int maxExistingConflictScore = max(numSets, conflictScores);
            if (newConflictScores[i] < maxExistingConflictScore) {
                int j = getIndex(numSets, conflictScores, maxExistingConflictScore);
                boards[j] = newBoards[i];
                conflictScores[j] = newConflictScores[i];
            }
        }

        if (randDouble() < genDivFactor) {
            mutateTrulyRandom(N, boards[rand() % numSets]);
        }

        if (iter % 1000 == 0) {
            printf("Conflict Scores: ");
            for (int i = 0; i < numSets; i++) {
                printf("%d ", conflictScores[i]);
            }
            printf("\n");
        }

        iter++;
    }

    for (int i = 0; i < numSets; i++) {
        printf("Board %d | Conflic Score: %d\n", i, conflictScores[i]);
        printBoard(N, boards[i]);
    }

    printf("Solved in %d iterations\n", iter);
}
