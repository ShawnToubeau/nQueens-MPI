#include <stdlib.h>
#include <stdio.h>
#include "omp.h"

int sum(int arr[], int N)
{
    int sum = 0; // initialize sum

    // Iterate through all elements
    // and add them to sum
    for (int i = 0; i < N; i++)
        sum += arr[i];

    return sum;
}

// Calculates the conflict scores of a chess board
int computeConflictScore(int N, int board[N])
{
    int i = 0, j = 1; // Double iterators
    int(*conflictQueens) = malloc(sizeof(int[N]));

    // Set all values in conflict queen to -1
    for (; i < N * N; i++)
    {
        conflictQueens[i] = -1;
    }

    while (i != N * N)
    {

        if (j == N * N - 1)
        { // Reached all comparisons
            if (conflictQueens[i] == -1)
            {
                conflictQueens[i] = 0;
            }
            break;
        }

        // Check diagonals and column
        if (i - board[i] == j - board[j] || i + board[i] == j + board[j] || board[i] == board[j])
        {
            conflictQueens[i] = 1, conflictQueens[j] = 1; // Found conflict
            if (j == N * N - 1)
            { // Special case where conflict is with last positioned K/M
                i++;
                j = i + 1;
            }
            else
            { // Case where conflict occurs before the last positioned K/M
                j++;
            }
        }
        else if (j == N * N - 1)
        { // If K/M is at the last postion
            if (conflictQueens[i] == -1)
            {
                conflictQueens[i] = 0;
            }
            i++;
            j = i + 1;
        }
        else
        {
            j++;
        }
    }

    return sum(conflictQueens, N);
}

int computeRandMutation(int N, int board[N])
{
    return 0;
}
