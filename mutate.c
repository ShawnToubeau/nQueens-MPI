#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "mutate.h"
#include "conflict.h"
#include "assistFunctions.h"

/*
  Unused in favor of mutateMaxConflict
 */
void mutate(int N, int* board) {
  int* conflictsPerQueen = getConflictsPerQueen(N, board);
  int sampleIndex = rand() % N;
  int sampleConflict = conflictsPerQueen[sampleIndex];

  if (max(N, conflictsPerQueen) == 0) {
    return;
  } else {
    while (sampleConflict == 0) {
      sampleIndex = rand() % N;
      sampleConflict = conflictsPerQueen[sampleIndex];
    }
  }

  int posC = 0;
  int conflict, minConflict, minConflictIndex;

  while (posC < N) {
    board[sampleIndex] = posC;
    conflict = getConflictsPerQueen(N, board)[sampleIndex];

    if (conflict < minConflict) {
      minConflict = conflict;
      minConflictIndex = posC;
    }

    posC++;
  }

  board[sampleIndex] = minConflictIndex;
}

/*
  This method will mutate the board reference passed to it directly.
 */
void mutateTrulyRandom(int N, int* board) {
  int i = rand() % N;
  int c = rand() % N;
  
  board[i] = c;
}

/*
  Finds queen in maximum conflict and trys to improve that individual queen's conflict score
 */
void mutateMaxConflict(int N, int* board) {
  int *conflictsPerQueen = getConflictsPerQueen(N, board);

  int maxConflict = max(N, conflictsPerQueen);

  if (maxConflict == 0) return;
  int maxConflictIndex = getIndex(N, conflictsPerQueen, maxConflict);

  int posC = 0;
  int minConflict = INT_MAX;
  int conflict, minConflictIndex;

  while (posC < N) {
    board[maxConflictIndex] = posC;
    conflict = getConflictsPerQueen(N, board)[maxConflictIndex];

    if (conflict < minConflict) {
      minConflict = conflict;
      minConflictIndex = posC;
    }

    posC++;
  }

  board[maxConflictIndex] = minConflictIndex;
}

/*
  This method will not mutate the board references passed to it. Instead
  it will return two new boards identical in size too the ones passed in.
 */
void crossoverRandomSplit(int N, int* board1, int* board2, double r, int** newBoards) {
  // default to 0.15
  if (r < 0) {
    r = 0.15;
  }

  int splitIndex = rand() % N;

  for (int i = 0; i < N; i++) {
    if (i < splitIndex) {
      newBoards[0][i] = board1[i];
      newBoards[1][i] = board2[i];
    } else {
      newBoards[0][i] = board2[i];
      newBoards[1][i] = board1[i];
    }
  }

  if (randDouble() < r) {
    mutateTrulyRandom(N, newBoards[0]);
    mutateTrulyRandom(N, newBoards[1]);
  }
}
