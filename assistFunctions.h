#ifndef AF_H_ /* Include guard */
#define AF_H_

// Used in main.c
int min(int N, int* l);

// Used in mutate.c, main.c
int max(int N, int* l);
int getIndex(int N, int* l, int m);

// Used in mutate.c
double randDouble();

// Used in conflict.c
int sum(int n, int *arr);

// Used in test.c
void printBoard(int N, int *board);
void fillRandom (int N, int* board);

#endif