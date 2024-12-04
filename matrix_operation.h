#ifndef MATRIX_SOLVER_H
#define MATRIX_SOLVER_H

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

            // Вводимо елементи матриці для поточного рядка
            for (int j = 0; j < rows; j++) {
                A[i][j] = get_double_input("Enter value: ", -100, 100);
                if (i == j) {
                    diagonal_element = fabs(A[i][j]);  // Діагональний елемент
                } else {
                    off_diagonal_sum += fabs(A[i][j]);  // Сума елементів, що не належать до діагоналі
                }
            }

            // Перевірка на діагональне домінування
            if (diagonal_element <= off_diagonal_sum) {
                printf("Invalid row: A[%d][%d] = %.*lf is not greater than the sum of other elements = %.*lf.\n",
                       i + 1, i + 1, precision, diagonal_element, precision, off_diagonal_sum);
                printf("Please re-enter the entire row #%d.\n", i + 1);
            }
        } while (diagonal_element <= off_diagonal_sum);  // Якщо не виконано, повторюємо введення для цього рядка

        // Вводимо вектор b
        printf("Enter b[%d]:\n", i + 1);
        b[i] = get_double_input("Enter value: ", -100, 100);
    }
}

// Solve SLAE using an iterative method
int solve_system(double **matrix, double *vector, double *prev_sol, double *current_sol, int size, double epsilon) {
    double max_diff = 0.0;
    int iterations = 0, max_iterations = 1000;

    // Ітерації методу
    do {
        for (int i = 0; i < size; i++) {
            double sum = 0.0;
            for (int j = 0; j < size; j++) {
                if (j != i) sum += matrix[i][j] * prev_sol[j];
            }
            current_sol[i] = (vector[i] - sum) / matrix[i][i];

            // Визначення максимальної зміни серед компонент вектора
            double diff = fabs(current_sol[i] - prev_sol[i]);
            if (diff > max_diff) max_diff = diff;
        }

        // Оновлення попереднього розв'язку
        for (int i = 0; i < size; i++) prev_sol[i] = current_sol[i];

        iterations++;  // Лічильник ітерацій

        // Перевірка на перевищення максимальної кількості ітерацій
        if (iterations >= max_iterations) {
            printf("The method did not converge within the maximum number of iterations (%d).\n", max_iterations);
            return 0;  // Не зійшлося
        }
    } while (max_diff > epsilon);  // Порівняння з точністю

    // Якщо рішення зійшлося
    printf("Converged in %d iterations.\n", iterations);
    return 0;
}

void verify_solution(double **A, double *b, int rows, int precision) {
    printf("\nVerification of the solution (equations):\n");
    for (int i = 0; i < rows; i++) {
        printf("Row %d: ", i + 1);
        for (int j = 0; j < rows; j++) {
            printf("%.*lf * x[%d] ", precision, A[i][j], j + 1);
            if (j < rows - 1) {
                printf("+ ");
            }
        }
        printf("= %.*lf\n", precision, b[i]);
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
