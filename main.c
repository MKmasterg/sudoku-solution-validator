#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* evaluate_rows(void* arg) {
    // This function evaluates the rows of sudoku
    // returns 1 if the rows are valid, 0 otherwise
    // arg is a pointer to the sudoku board
    int flag = 1;
    int seen[9] = {0};
    int *board = (int*)arg;
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++) {
            if (seen[board[i*9 + j]] == 0){
                seen[board[i*9 + j]] = 1;
            } else {
                flag = 0;
                break;
            }
        }

        if (flag == 0){
            break;
        }

        for (int k = 0; k < 9; k++){
            seen[k] = 0;
        }

    }
    if (flag == 1){
        return (void*)1;
    } else {
        return (void*)0;
    }
}

void* evaluate_columns(void* arg) {
    // This function evaluates the columns of sudoku
    // returns 1 if the columns are valid, 0 otherwise
    // arg is a pointer to the sudoku board
    int flag = 1;
    int seen[9] = {0};
    int *board = (int*)arg;
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++) {
            if (seen[board[j*9 + i]] == 0){
                seen[board[j*9 + i]] = 1;
            } else {
                flag = 0;
                break;
            }
        }

        if (flag == 0){
            break;
        }

        for (int k = 0; k < 9; k++){
            seen[k] = 0;
        }

    }
    if (flag == 1){
        return (void*)1;
    } else {
        return (void*)0;
    }
}

void* evaluate_box(void* arg) {
    // This function evaluates the given box of sudoku
    // returns 1 if the box is valid, 0 otherwise
    // arg is a pointer to the specified box
    int flag = 1;
    int seen[9] = {0};
    int *box = (int*)arg;

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++) {
            if (seen[box[i*3 + j]] == 0){
                seen[box[i*3 + j]] = 1;
            } else {
                flag = 0;
                break;
            }
        }

        if (flag == 0){
            break;
        }

        for (int k = 0; k < 9; k++){
            seen[k] = 0;
        }
    }
    if (flag == 1){
        return (void*)1;
    } else {
        return (void*)0;
    }
}

int main(void) {
    // TODO : get the sudoku board from the user
    int sudoku[9][9] = {
            {5, 3, 4, 6, 7, 8, 9, 1, 2},
            {6, 7, 2, 1, 9, 5, 3, 4, 8},
            {1, 9, 8, 3, 4, 2, 5, 6, 7},
            {8, 5, 9, 7, 6, 1, 4, 2, 3},
            {4, 2, 6, 8, 5, 3, 7, 9, 1},
            {7, 1, 3, 9, 2, 4, 8, 5, 6},
            {9, 6, 1, 5, 3, 7, 2, 8, 4},
            {2, 8, 7, 4, 1, 9, 6, 3, 5},
            {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };


    pthread_t threads[27];
    int threadIndex = 0;
    int results[27];

    // Create threads for rows
    pthread_create(&threads[threadIndex++], NULL, evaluate_rows, (void*)sudoku);

    // Create threads for columns
    pthread_create(&threads[threadIndex++], NULL, evaluate_columns, (void*)sudoku);

    // Create threads for 3x3 boxes
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int *box = malloc(9 * sizeof(int));
            for (int x = 0; x < 3; x++) {
                for (int y = 0; y < 3; y++) {
                    box[x * 3 + y] = sudoku[i * 3 + x][j * 3 + y];
                }
            }
            pthread_create(&threads[threadIndex++], NULL, evaluate_box, (void*)box);
        }
    }

    // Join threads and collect results
    for (int i = 0; i < 27; i++) {
        void *result;
        pthread_join(threads[i], &result);
        results[i] = (int)(long)result;
    }

    // Check results
    int isValid = 1;
    for (int i = 0; i < 27; i++) {
        if (results[i] == 0) {
            isValid = 0;
            break;
        }
    }

    if (isValid) {
        printf("The Sudoku solution is valid.\n");
    } else {
        printf("The Sudoku solution is invalid.\n");
    }

    return 0;
}
