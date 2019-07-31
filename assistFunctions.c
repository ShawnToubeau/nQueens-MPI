#include <stdlib.h>
#include <stdio.h>
#include "assistFunctions.h"

int max(int N, int* l) {
  int m = INT8_MIN;
  for (int i = 0; i < N; i++) {
    if (l[i] > m) m = l[i];
  }
  return m;
}

int getIndex(int N, int* l, int m) {
  int i = -1;
  for (int j = 0; j < N; j++) {
    if (l[j] == m) {
      i = j;
      break;
    }
  }
  return i;
}

double randDouble() {
  return (double)rand()/(double)RAND_MAX;
}

int sum(int N, int *arr)
{
    int sum = 0; // initialize sum

    // Iterate through all elements
    // and add them to sum
    for (int i = 0; i < N; i++) {
        sum += arr[i];
    }

    return sum;
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

void fillRandom (int N, int* board) {
  for (int i = 0; i < N; ++i)
  {
    board[i] = rand() % N;
  }
}
