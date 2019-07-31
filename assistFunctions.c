#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

/*  Assuming board[][] contains one of two values:
 *      0: No queen present
 *      1: Queen present
 */ 

int computeConflictScore(int x, int y, int** board) {
    int conflictScore = 0;
    #pragma omp parallel default(shared)
    {
        for(int i = omp_get_num_threads(); i < y; i+=omp_get_thread_num()) {
            for(int j = 0; j < x; j++) {
                if(board[i][j] == 1) {
                    for(int k = 0; k < x; k++) {
                        if(board[i][k] == 1 && k != j) { // horizontal checks
                            #pragma omp atomic update
                            conflictScore++;
                        }
                        if(board[k][j] == 1 && k != i) { // vertical checks
                            #pragma omp atomic update
                            conflictScore++;
                        }
                        if(i-k >= 0 && j-k >= 0 && i+k < y && j+k < x && k != i && k != j) { // diagonal checks
                            if(board[i-k][j-k] == 1)
                                #pragma omp atomic update
                                conflictScore++;
                            if(board[i+k][j-k] == 1)
                                #pragma omp atomic update
                                conflictScore++;
                            if(board[i-k][j+k] == 1)
                                #pragma omp atomic update
                                conflictScore++;
                            if(board[i+k][j+k] == 1)
                                #pragma omp atomic update
                                conflictScore++;
                        }
                    }
                }
            }
        }
    }
    return conflictScore/2;
}

int computeRandMutation(int x, int y,, int** board) {
    return 0;
}

int main() {
    return 0;
}
