#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mutation.h"
#include "conflict.h"

int max(int N, int* l) {
  int m;
  for (int i = 0; i < N; i++) {
    if (l[i] > m) m = l[i];
  }
  return m;
}

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
  This method will not mutate the board references passed to it. Instead
  it will return two new boards identicle in size too the ones passed in.
 */
int** crossoverRandomSplit(int N, int* board1, int* board2, double r) {
  int** res = (int **)malloc(2 * N * sizeof(int));
  res[0] = (int *)malloc(N * sizeof(int));
  res[1] = (int *)malloc(N * sizeof(int));

  // default to 0.15
  if (r < 0) {
    r = 0.15;
  }

  int splitIndex = rand() % N;

  for (int i = 0; i < N; i++) {
    if (i < splitIndex) {
      res[0][i] = board1[i];
      res[1][i] = board2[i];
    } else {
      res[0][i] = board2[i];
      res[1][i] = board1[i];
    }
  }

  if (randDouble() < r) {
    mutateTrulyRandom(N, res[0]);
    mutateTrulyRandom(N, res[1]);
  }

  return res;
}

double randDouble() {
  return (double)rand()/(double)RAND_MAX;
}
