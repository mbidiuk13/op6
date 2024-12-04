#ifndef RANDOM_MATRIX_GENERATOR_H
#define RANDOM_MATRIX_GENERATOR_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void generate_random_matrix(double **A, double *b, int rows, double min, double max) {
    for (int i = 0; i < rows; i++) {
        double off_diagonal_sum = 0.0;
        for (int j = 0; j < rows; j++) {
            if (i != j) {
                A[i][j] = (max - min) * ((double)rand() / RAND_MAX) + min;
                off_diagonal_sum += fabs(A[i][j]);
            }
        }
        A[i][i] = off_diagonal_sum + (max - min) * ((double)rand() / RAND_MAX) + 1.0;
        b[i] = (max - min) * ((double)rand() / RAND_MAX) + min;
    }
}

#endif
