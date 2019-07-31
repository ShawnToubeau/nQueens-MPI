#ifndef MUTATION_H_
#define MUTATION_H_

void mutateTrulyRandom(int N, int* board);
int** crossoverRandomSplit(int N, int* board1, int* board2, double r);
void printBoard (int N, int* board);
void fillRandom (int N, int* board);
double randDouble();
void testMutateTrulyRandom();
void testCrossoverRandomSplit();

#endif