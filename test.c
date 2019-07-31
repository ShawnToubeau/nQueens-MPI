#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mutate.h"
#include "conflict.h"
#include "test.h"

int main(int argc, char **argv) {

  srand(time(0));

  // testCrossoverRandomSplit();

  // testGetConflictsPerQueen();

  // testMax();
  // testGetIndex();

  // testMutate();
  // testMutateMaxConflict();
  // testMutateTrulyRandom();
}

void printBoard (int N, int* board) {
  printf("\n");
  for (int i = 0; i < N; ++i)
  {
    printf("[");
    for (int j = 0; j < N; ++j)
    {
      if (board[i] == j) {
        printf("Q");
      } else {
        printf("+");
      }
    }
    printf("]\n");
  }
  printf("\n");
}

void fillRandom (int N, int* board) {
  for (int i = 0; i < N; ++i)
  {
    board[i] = rand() % N;
  }
}

void testCrossoverRandomSplit() {
  int N = 4;

  // Allocate
  int *board1 = malloc(N * sizeof(int));
  int *board2 = malloc(N * sizeof(int));
  int** res = (int **)malloc(2 * N * sizeof(int));
  res[0] = (int *)malloc(N * sizeof(int));
  res[1] = (int *)malloc(N * sizeof(int));

  // Assign
  fillRandom(N, board1);
  fillRandom(N, board2);

  // Print before
  printf("Before Crossover Random Split:\n");
  printf("Board 1:\n");
  printBoard(N, board1);
  printf("Board 2:\n");
  printBoard(N, board2);

  double randFactor = 0; // less than 0 defaults to 0.15, should not exceed 1.
  res = crossoverRandomSplit(N, board1, board2, randFactor);

  printf("After Crossover Random Split:\n");
  printf("Result 1:\n");
  printBoard(N, res[0]);
  printf("Result 2:\n");
  printBoard(N, res[1]);

  free(board1);
  free(board2);
  free(res[0]);
  free(res[1]);
  free(res);
}

void testGetConflictsPerQueen() {
  int N = 4;
  int board[4] = { 0, 0, 0, 0 };

  int* conflictScoresPerQueen = getConflictsPerQueen(N, board);
  printBoard(N, board);
  for (int i = 0; i < N; i++) {
    printf("Conflicts for Queen %d = %d\n", i, conflictScoresPerQueen[i]);
  }
}

void testMax() {
  int l[5] = { 0, 99, 15, -23, 100 };

  int m = max(5, l);

  printf("Max is: %d\n", m);
}

void testGetIndex() {
  int l[5] = { 0, 99, 15, -23, 100 };

  int i = getIndex(5, l, 15);

  printf("Index is: %d\n", i);
}

void testMutate() {
  int N = 4;
  int board[4] = { 0, 0, 0, 0 };

  printf("Before Mutation\n");
  printBoard(N, board);

  mutate(N, board);

  printf("After Mutation\n");
  printBoard(N, board);
}

void testMutateMaxConflict() {
  int N = 4;
  int board[4] = { 1, 0, 3, 1 };

  printf("Before Mutation\n");
  printBoard(N, board);

  mutateMaxConflict(N, board);

  printf("After Mutation\n");
  printBoard(N, board);
}

void testMutateTrulyRandom() {
  int N = 4;

  // Allocate
  int *board = malloc(N * sizeof(int));

  // Assign
  fillRandom(N, board);

  // Print before
  printf("Before Mutation:\n");
  printBoard(N, board);

  for (int i = 1; i < 5; i++) {
    mutateTrulyRandom(N, board);
    printf("After Mutation %d:\n", i);
    printBoard(N, board);
  }

  free(board);
}