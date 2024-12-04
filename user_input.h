#ifndef INPUT_VALIDATION_H
#define INPUT_VALIDATION_H

#include <stdio.h>

int get_integer_input(const char *prompt, int min, int max) {
    int value = 0;
    do {
        printf("%s", prompt);
        int input = scanf("%d", &value);
        if (input != 1 || value < min || value > max) {
            printf("Invalid input. Please enter a valid number.\n");
            fflush(stdin);
        }
    } while (value < min || value > max);
    fflush(stdin);
    return value;
}

double get_double_input(const char *prompt, double min, double max) {
    double value = 0.0;
    int input = 0;
    do {
        printf("%s", prompt);
        input = scanf("%lf", &value);
        if (input != 1 || value < min || value > max) {
            printf("Invalid input. Please enter a number between 1e-12 and 1e-1.\n", min, max);
            fflush(stdin);
        }
    } while (input != 1 || value < min || value > max);
    return value;
}

char get_char_input(const char *prompt, char choice_1, char choice_2) {
    char user_choice;
    do {
        printf("%s", prompt);
        int input = scanf(" %c", &user_choice);  //пробіл перед %c, щоб пропустити можливі символи нового рядка
        if (input != 1 || (user_choice != choice_1 && user_choice != choice_2)) {
            printf("Invalid input.\n");
            fflush(stdin);
        }
    } while (user_choice != choice_1 && user_choice != choice_2);
    fflush(stdin);
    return user_choice;
}

#endif
