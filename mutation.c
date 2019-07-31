#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mutateTrulyRandom(int N, int* board) {
  int i = rand() % N;
  int c = rand() % N;
  
  board[i] = c;
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

int main(int argc, char **argv) {

  srand(time(0));

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
}