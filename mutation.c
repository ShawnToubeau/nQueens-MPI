#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mutateTrulyRandom(int N, int* board);
int** crossoverRandomSplit(int N, int* board1, int* board2, double r);
void printBoard (int N, int* board);
void fillRandom (int N, int* board);
double randDouble();
void testMutateTrulyRandom();
void testCrossoverRandomSplit();

int main(int argc, char **argv) {

  srand(time(0));

  // testMutateTrulyRandom();
  testCrossoverRandomSplit();
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

double randDouble() {
  return (double)rand()/(double)RAND_MAX;
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
