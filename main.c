#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "matrix_operation.h"
#include "user_input.h"
#include "random.h"

#define MIN_ROWS 2
#define MAX_ROWS 15
#define MIN_RANDOM_VALUE -100
#define MAX_RANDOM_VALUE 100

int main() {
    do {
        srand(time(NULL));
        printf("=== SLAE solver ===\n");

        int rows = get_integer_input("Enter matrix size (2 to 15):", MIN_ROWS, MAX_ROWS);
        double epsilon = get_double_input("Enter epsilon (1e-12 to 1e-1):", 1e-12, 1e-1); //допустимий рівень похибки при розв'язуванні системи
        int precision = get_integer_input("Enter the precision for output (1 to 12):", 1, 12);  //точність для виведення

        //виділення пам'яті
        double *b = allocate_vector(rows);
        double *x = allocate_vector(rows);
        double *x_prev = allocate_vector(rows);
        double **A = allocate_matrix(rows);

        if (!b || !x || !x_prev || !A) {
            printf("Memory allocation error!\n");
            free_all(b, x, x_prev, A, rows);
            return 0;
        }

        char input_choice = get_char_input("Choose mode: 'a' for auto-fill, 'm' for manual entry:", 'a', 'm');
        switch (input_choice) {
            case 'm':
                input_matrix_manually(A, b, rows, 5);
                break;
            case 'a':
                printf("\nAuto-filling matrix and vector with random values...\n");
                generate_random_matrix(A, b, rows, MIN_RANDOM_VALUE, MAX_RANDOM_VALUE);
                break;
            default:
                printf("Invalid choice!\n");
                free_all(b, x, x_prev, A, rows);
                return 0;
        }

        //виведення матриці і вектора
        printf("\nMatrix and vector:\n");
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < rows; j++) {
                printf("%*.*lf ", precision + 5, precision, A[i][j]);
            }
            printf("| %.*lf\n", precision, b[i]);
        }

        //рішення системи рівнянь
        printf("\nStarting iterative solver...\n");
        solve_system(A, b, x_prev, x, rows, epsilon);

        //виведення розв'язку
        printf("\nSolution found:\n");
        for (int i = 0; i < rows; i++) {
            printf("Solution[%d] = %.*lf\n", i + 1, precision, x[i]);
        }

        //перевірка розв'язку
        verify_solution(A, b, rows, precision);

        free_all(b, x, x_prev, A, rows);

        printf("Press '0' to exit or any other key to continue\n");
    } while (getchar() != '0');

    return 0;
}
