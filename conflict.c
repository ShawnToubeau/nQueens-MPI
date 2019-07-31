#include <stdio.h>
#include <stdlib.h>
#include "conflict.h"
#include "assistFunctions.h"

int* getConflictsPerQueen(int N, int *board)
{
  int *conflictsPerQueen = (int *)malloc(N * sizeof(int));
  for (int i = 0; i < N; i++)
  {
    conflictsPerQueen[i] = 0;
  }

  int i = 0, j = 1;
  while (i != N - 1)
  {
    if (i - board[i] == j - board[j] || i + board[i] == j + board[j] || board[i] == board[j])
    {
      conflictsPerQueen[i]++;
      conflictsPerQueen[j]++;
    }

    if (j == N - 1)
    {
      i++;
      j = i + 1;
    }
    else
    {
      j++;
    }
  }

  return conflictsPerQueen;
}

// Calculates the conflict scores of a board
int computeConflictScore(int N, int *board)
{
  // Board to store all out conflicts
  int *conflictQueens = (int *)malloc(N * sizeof(int));

  // Set all values in the conflict board to -1
  for (int i = 0; i < N; i++)
  {
    conflictQueens[i] = -1;
  }

  // Define double iterators
  int i = 0, j = 1;
  // Loop through the entire board
  while (i != N)
  {
    // Check if we've reached all comparisons
    if (i == N - 1)
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
    }

    if (j == N - 1) // Special case where conflict is with last positioned j
    {
      if (conflictQueens[i] == -1)
      {
        conflictQueens[i] = 0;
      }
      i++;
      j = i + 1;
    }
    else // Case where conflict occurs before the last positioned j
    {
      j++;
    }
  }
  // Return summed conflict scores
  return sum(N, conflictQueens);
}
