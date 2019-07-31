#include <stdlib.h>
#include <stdio.h>
#include "omp.h"
#include "assistFunctions.h"

int sum(int arr[], int N)
{
    int sum = 0; // initialize sum

    // Iterate through all elements
    // and add them to sum
    for (int i = 0; i < N; i++)
        sum += arr[i];

    return sum;
}

// Calculates the conflict scores of a board
int computeConflictScore(int N, int *board)
{
    // Define double iterators
    int i = 0, j = 1;
    // Board to store all out conflicts
    int *conflictQueens = (int *)malloc(N * N * sizeof(int));

    // Set all values in the conflict board to -1
    for (; i < N * N; i++)
    {
        conflictQueens[i] = -1;
    }

    // Reset i
    i = 0;
    // Loop through the entire board
    while (i != N * N)
    {
        // Check if we've reached all comparisons
        if (j == N * N - 1)
        {
            if (conflictQueens[i] == -1)
            {
                conflictQueens[i] = 0;
            }
            break;
        }

        // Check diagonals, columns, and rows
        if (i - board[i] == j - board[j] || i + board[i] == j + board[j] || board[i] == board[j])
        {
            conflictQueens[i] = 1, conflictQueens[j] = 1; // Found conflict
            // Special case where conflict is with last positioned j
            if (j == N * N - 1)
            {
                // Increment our iterators
                i++;
                j = i + 1;
            }
            // Case where conflict occurs before the last positioned j
            else
            {
                // Increment j
                j++;
            }
        }
        // Check if j has reached the end
        else if (j == N * N - 1)
        {
            // Checks if there's an existing conflict
            if (conflictQueens[i] == -1)
            {
                conflictQueens[i] = 0;
            }
            // Increment out iterators
            i++;
            j = i + 1;
        }
        else
        {
            // Increment j
            j++;
        }
    }
    // Return summed conflict scores
    return sum(conflictQueens, N);
}

int computeRandMutation(int N, int *board)
{
    return 0;
}

void printBoard(int N, int *board)
{
    printf("\n");
    for (int i = 0; i < N; ++i)
    {
        printf("[");
        for (int j = 0; j < N; ++j)
        {
            if (board[i] == j)
            {
                printf("Q");
            }
            else
            {
                printf("+");
            }
        }
        printf("]\n");
    }
    printf("\n");
}
