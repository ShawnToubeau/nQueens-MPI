#include "assistFunctions.h"
#include "conflict.h"
#include "mutate.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void slave(int N, int *board1, int *board2, int** newBoards)
{
    crossoverRandomSplit(N, board1, board2, 0.15, newBoards);
    newBoards[2] = board1;
    newBoards[3] = board2;
    mutateMaxConflict(N, newBoards[2]);
    mutateMaxConflict(N, newBoards[3]);
}

int main(int argc, char **argv)
{ 
    srand(time(NULL));
    int numSets = 4;
    int N = 64;
    double genDivFactor = 0.3;

    int** boards = (int **)malloc(numSets * N * sizeof(int));
    int** newBoards = (int **)malloc(4 * N * sizeof(int));

    int* conflictScores = (int *)malloc(numSets * sizeof(int));
    int* newConflictScores = (int *)malloc(4 * sizeof(int));

    for (int i = 0; i < numSets; i++) {
        boards[i] = (int *)malloc(N * sizeof(int));
        fillRandom(N, boards[i]);
        conflictScores[i] = computeConflictScore(N, boards[i]);
        printf("Board %d | Conflic Score: %d\n", i, conflictScores[i]);
        // printBoard(N, boards[i]);
    }

    for (int i = 0; i < 4; i++) {
        newBoards[i] = (int *)malloc(N * sizeof(int));
    }

    int iter = 0;
    while (sum(numSets, conflictScores) != 0) {
        int i1 = rand() % numSets;
        int i2;
        do {
            i2 = rand() % numSets;
        } while(i2 == i1);

        slave(N, boards[i1], boards[i2], newBoards);
        for (int i = 0; i < 4; i++) {
            newConflictScores[i] = computeConflictScore(N, newBoards[i]);
        }

        for (int i = 0; i < 4; i++) {
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
        // printBoard(N, boards[i]);
    }

    printf("Solved in %d iterations\n", iter);
}
