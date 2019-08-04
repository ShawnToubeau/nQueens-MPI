#ifndef MUTATION_H_
#define MUTATION_H_

void mutate(int N, int* board);
void mutateTrulyRandom(int N, int* board);
void mutateMaxConflict(int N, int* board);

int** crossoverRandomSplit(int N, int* board1, int* board2, double r);


#endif
