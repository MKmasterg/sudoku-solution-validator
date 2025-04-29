#include <pthread.h>
#include <stdio.h>

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



int main(void) {
    return 0;
}
