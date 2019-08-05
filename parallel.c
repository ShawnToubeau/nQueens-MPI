#include "assistFunctions.h"
#include "conflict.h"
#include "mutate.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "mpi.h"

void slave(int N, int *board1, int *board2, int** newBoards)
{
    // Mutation algorithms
    crossoverRandomSplit(N, board1, board2, 0.15, newBoards);
    newBoards[2] = board1;
    newBoards[3] = board2;
    mutateMaxConflict(N, newBoards[2]);
    mutateMaxConflict(N, newBoards[3]);
}

int main(int argc, char **argv)
{
    // Initialize MPI and MPI variables
    int rank, numranks;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int startTime, endTime, totalTime;
    startTime = MPI_Wtime();

    // Sets the seed of the random function
    srand(time(NULL));

    // Number of boards to solve for
    int numBoards = 4;
    // Size of each board
    int N = 64;
    // Factor for genetic diversity
    double genDivFactor = 0.3;

    // Arrays allocated on the master (rank 0)
    int** boards = (int **)malloc(numBoards * N * sizeof(int));
    int *conflictScores = (int *)malloc(numBoards * sizeof(int));
    int *bestBoard = (int *)malloc(N * sizeof(int));
    // Arrays allocated on the slaves (all other ranks)
    int *board1 = (int *)malloc(N * sizeof(int));
    int *board2 = (int *)malloc(N * sizeof(int));
    int **newBoards = (int **)malloc(4 * N * sizeof(int));

    int conflictScoreSum = 0;
    int bestConflictScore = INT_MAX;


    // Allocate the remaining space for the double pointer arrays
    for (int i = 0; i < numBoards; i++) {
        boards[i] = (int *)malloc(N * sizeof(int));
    }
    for (int i = 0; i < 4; i++) {
        newBoards[i] = (int *)malloc(N * sizeof(int));
    }

    // Master block
    if (rank == 0)
    {
        // Populates the boards and calculates the conflict scores
        for (int i = 0; i < numBoards; i++)
        {
            fillRandom(N, boards[i]);
            conflictScores[i] = computeConflictScore(N, boards[i]);
            conflictScoreSum += conflictScores[i];
            printf("Board %d | Conflict Score: %d\n", i, conflictScores[i]);
            // printBoard(N, boards[i]);
        }
    }

    // Send the sum of conflict score to every rank
    MPI_Bcast(&conflictScoreSum, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int iter = 0;
    // Loops while our boards are still in conflict
    while (conflictScoreSum != 0)
    {
        // Master block
        if (rank == 0)
        {
            // Random index variables
            int i1, i2;
            // Board send block
            for (int i = 1; i < numranks; i++)
            {
                i1 = rand() % numBoards;
                do
                {
                    i2 = rand() % numBoards;
                } while (i2 == i1);
                // Picks 2 of the boards randomly to send to the slaves
                MPI_Send(boards[i1], N, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(boards[i2], N, MPI_INT, i, 1, MPI_COMM_WORLD);
            }
            // Board receive block
            for (int i = 1; i < numranks; i++)
            {
                MPI_Recv(bestBoard, N, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&bestConflictScore, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                // Attempts to replace the current boards with better boards
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
            // Mutate one of the boards randomly depending on out genetic diversity factor
            if (randDouble() < genDivFactor)
            {
                mutateTrulyRandom(N, boards[rand() % numBoards]);
            }
            // Prints out update every 1,000 runs
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
        // Slave block
        else
        {
            // Receive the 2 boards from master
            MPI_Recv(board1, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(board2, N, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Apply the mutations
            slave(N, board1, board2, newBoards);

            int c;
            // Loop over the new boards we get
            for (int i = 0; i < 4; i++)
            {
                c = computeConflictScore(N, newBoards[i]);
                // Finds the board with the lowest conflict score
                if (c < bestConflictScore)
                {
                    bestConflictScore = c;
                    bestBoard = newBoards[i];
                }
            }
            // Return the best board with it's conflict score
            MPI_Send(bestBoard, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Send(&bestConflictScore, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);

        }

        // Update the conflict score value every rank has
        MPI_Bcast(&conflictScoreSum, 1, MPI_INT, 0, MPI_COMM_WORLD);

        iter++;
    }

    // printf("Rank %d roger roger\n", rank);
    // Synchronizes all the ranks
    MPI_Barrier(MPI_COMM_WORLD);

    endTime = MPI_Wtime();
    totalTime = endTime - startTime;

    // Prints out the solution
    if (rank == 0) {
        for (int i = 0; i < numBoards; i++)
        {
            printf("Board %d | Conflict Score: %d\n", i, conflictScores[i]);
            // printBoard(N, boards[i]);
        }
        printf("Solved in %d iterations in %d seconds\n", iter, totalTime);
    }

    // End the program
    MPI_Finalize();
    exit(0);
}
