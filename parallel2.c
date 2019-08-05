#include "assistFunctions.h"
#include "conflict.h"
#include "mutate.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "mpi.h"

int **slave(int N, int *board1, int *board2)
{

    // Holds the 4 newly mutated sets
    int **newBoards = (int **)malloc(4 * sizeof(int));
    int **crs = (int **)malloc(2 * N * sizeof(int));
    crs[0] = (int *)malloc(N * sizeof(int));
    crs[1] = (int *)malloc(N * sizeof(int));

    for (int i = 0; i < 4; i++)
    {
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
    int rank, numranks;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(time(NULL));

    int numBoards = 4;
    int N = 8;
    double genDivFactor = 0.3;

    // rank 0
    int** boards;
    int *conflictScores;
    int *bestBoard;
    // else
    int *board1;
    int *board2;
    int **newBoards;
    int conflictScoreSum = 0;

    if (rank == 0)
    {
        boards = (int **)malloc(numBoards * N * sizeof(int));
        conflictScores = (int *)malloc(numBoards * sizeof(int));

        for (int i = 0; i < numBoards; i++)
        {
            boards[i] = (int *)malloc(N * sizeof(int));
            fillRandom(N, boards[i]);
            conflictScores[i] = computeConflictScore(N, boards[i]);
            conflictScoreSum += conflictScores[i];
            printf("Board %d | Conflic Score: %d\n", i, conflictScores[i]);
            // printBoard(N, boards[i]);
        }
    } else {
        board1 = (int *)malloc(N * sizeof(int));
        board2 = (int *)malloc(N * sizeof(int));
        newBoards = (int **)malloc(4 * N * sizeof(int));
    }

    bestBoard = (int *)malloc(N * sizeof(int));
    int bestConflictScore = INT_MAX;

    MPI_Bcast(&conflictScoreSum, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int iter = 0;
    while (conflictScoreSum != 0)
    {
        // MPI_Bcast(&conflictScoreSum, 1, MPI_INT, 0, MPI_COMM_WORLD);
        // if (conflictScoreSum == 0) {
        //     break;
        // }

        if (rank == 0)
        {
            int i1, i2;
            for (int i = 1; i < numranks; i++)
            {
                i1 = rand() % numBoards;
                do
                {
                    i2 = rand() % numBoards;
                } while (i2 == i1);
                MPI_Send(boards[i1], N, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(boards[i2], N, MPI_INT, i, 1, MPI_COMM_WORLD);
            }
            for (int i = 1; i < numranks; i++)
            {
                MPI_Recv(bestBoard, N, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&bestConflictScore, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                for (int j = 0; j < numBoards; j++)
                {
                    if (bestConflictScore < conflictScores[j])
                    {
                        boards[j] = bestBoard;
                        conflictScoreSum -= conflictScores[j];
                        conflictScoreSum += bestConflictScore;
                        conflictScores[j] = bestConflictScore;
                    }
                }

                
            }
            if (randDouble() < genDivFactor)
            {
                mutateTrulyRandom(N, boards[rand() % numBoards]);
            }
            if (iter % 1000 == 0)
            {
                printf("Conflict Scores: ");
                for (int i = 0; i < numBoards; i++)
                {
                    printf("%d ", conflictScores[i]);
                }
                printf("\n");
            }
        }
        else
        {
            MPI_Recv(board1, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(board2, N, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            newBoards = slave(N, board1, board2);

            int c;
            for (int i = 0; i < 4; i++)
            {
                c = computeConflictScore(N, newBoards[i]);
                if (c < bestConflictScore)
                {
                    bestConflictScore = c;
                    bestBoard = newBoards[i];
                }
            }
            MPI_Send(bestBoard, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Send(&bestConflictScore, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);

        }

        MPI_Bcast(&conflictScoreSum, 1, MPI_INT, 0, MPI_COMM_WORLD);

        iter++;
    }

    printf("Rank %d roger roger\n", rank);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < numBoards; i++)
        {
            printf("Board %d | Conflic Score: %d\n", i, conflictScores[i]);
            // printBoard(N, boards[i]);
        }
        printf("Solved in %d iterations\n", iter);
    }
    MPI_Finalize();
}
