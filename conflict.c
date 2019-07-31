#include <stdio.h>
#include <stdlib.h>
#include "conflict.h"

int* getConflictsPerQueen(int N, int* board) {

  int* conflictsPerQueen = (int *)malloc(N * sizeof(int));
  for (int i = 0; i < N; i++) {
    conflictsPerQueen[i] = 0;
  }

  int i = 0, j = 1;
  while (i != N - 1) {
    if (i - board[i] == j - board[j] || i + board[i] == j + board[j] || board[i] == board[j]) {
      conflictsPerQueen[i]++;
      conflictsPerQueen[j]++;
    }

    if (j == N - 1) {
      i++;
      j = i + 1;
    } else {
      j++;
    }
  }

  return conflictsPerQueen;
}