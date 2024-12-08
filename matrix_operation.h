#ifndef MATRIX_OPERATION_H
#define MATRIX_OPERATION_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "user_input.h"

double *allocate_vector(int size) {
    return calloc(size, sizeof(double));
}

double **allocate_matrix(int size) {
    double **matrix = calloc(size, sizeof(double *));
    for (int i = 0; i < size; i++) {
        matrix[i] = calloc(size, sizeof(double));
    }
    return matrix;
}

void input_matrix_manually(double **A, double *b, int rows, int precision) {
    for (int i = 0; i < rows; i++) {
        double diagonal_element = 0.0;
        double off_diagonal_sum = 0.0;
        do {
            printf("Enter %d element(s) for row #%d\n", rows, i + 1);

            //введення елементів матриці для поточного рядка
            for (int j = 0; j < rows; j++) {
                A[i][j] = get_double_input("Enter value: ", -100, 100);
                if (i == j) {
                    diagonal_element = fabs(A[i][j]);
                } else {
                    off_diagonal_sum += fabs(A[i][j]);
                }
            }

            //перевірка на діагональне домінування
            if (diagonal_element <= off_diagonal_sum) {
                printf("Invalid row: A[%d][%d] = %.*lf is not greater than the sum of other elements = %.*lf.\n",
                       i + 1, i + 1, precision, diagonal_element, precision, off_diagonal_sum);
                printf("Please re-enter the entire row #%d.\n", i + 1);
            }
        } while (diagonal_element <= off_diagonal_sum);

        //введення вектор b
        printf("Enter b[%d]:\n", i + 1);
        b[i] = get_double_input("Enter value: ", -100, 100);
    }
}

//розв'язання слар, використовуючи ітеративний метод
void solve_slae(double **a, const double *b, double *x, double *xp, const unsigned int size, const double epsilon) {
    int iteration = 0;
    double max_delta = 0.0;
    do {
        iteration++;

        for (int i = 0; i < size; i++) {
            xp[i] = 0.0;
            for (int j = 0; j < size; j++) {
                if (i != j) {
                    xp[i] = xp[i] + a[i][j] * x[j];
                }
            }
            xp[i] = (b[i] - xp[i]) / a[i][i];
        }

        for (int i = 0; i < size; i++) {
            if (fabs(x[i] - xp[i]) > max_delta) {
                max_delta = fabs(x[i] - xp[i]);
            }
            x[i] = xp[i];
        }

    } while (max_delta > epsilon);

    printf("Converged in %d iterations.\n", iteration);
}

void verify_solution(double **A, double *x, int rows, int precision) {
    printf("\nVerification of the solution:\n");

    for (int i = 0; i < rows; i++) {
        double calculated_b = 0.0;

        printf("Row %d: ", i + 1);
        for (int j = 0; j < rows; j++) {
            double product = A[i][j] * x[j];
            calculated_b += product;

            if (j > 0) {
                printf(" + ");
            }

            //форматування множників із дужками
            printf("%.*lf*(%.*lf)", precision, A[i][j], precision, x[j]);
        }

        printf(" = %.*lf\n", precision, calculated_b);
    }
}

void free_all(double *b, double *x, double *x_prev, double **A, int rows) {
    free(x_prev);
    free(x);
    free(b);
    for (int i = 0; i < rows; i++) {
        free(A[i]);
    }
    free(A);
}

#endif
