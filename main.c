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
            if (seen[board[i*9 + j] - 1] == 0){
                seen[board[i*9 + j] - 1] = 1;
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
    pthread_exit((void*)(long)flag);
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
            if (seen[board[j*9 + i] - 1] == 0){
                seen[board[j*9 + i] - 1] = 1;
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
    pthread_exit((void*)(long)flag);
}

void* evaluate_box(void* arg) {
    // This function evaluates the given box of sudoku
    // returns 1 if the box is valid, 0 otherwise
    // arg is a pointer to the specified box
    int flag = 1;
    int seen[9] = {0};
    int *box = (int*)arg;

    for (int i = 0; i < 9; i++) {
        int val = box[i];
        if (val < 1 || val > 9 || seen[val - 1]) {
            flag = 0;
            break;
        }
        seen[val - 1] = 1;
    }

    free(box);
    pthread_exit((void*)(long)flag);
}

int user_input(int sudoku[9][9]) {
    // This function gets the sudoku board from the user
    // returns 1 if the user provides a board, 0 otherwise
    int defFlag = 0;
    printf("Do you want to enter a Sudoku board? (1 for yes, 0 for no): ");
    scanf("%d", &defFlag);
    if (defFlag == 1) {
        printf("Enter the Sudoku board (0 for empty cells):\n");
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                scanf("%d", &sudoku[i][j]);
            }
        }
    }
    return defFlag;
}

int main(void) {
    // Get the sudoku board from the user, if the user doesn't provide a board, use the default board
    int sudoku[9][9] = {0};
    int defFlag = 0;
    defFlag = user_input(sudoku);

    // default Sudoku board
    int defSudoku[9][9] = {
            {6, 2, 4, 5, 3, 9, 1, 8, 7},
            {5, 1, 9, 7, 2, 8, 6, 3, 4},
            {8, 3, 7, 6, 1, 4, 2, 9, 5},
            {1, 4, 3, 8, 6, 5, 7, 2, 9},
            {9, 5, 8, 2, 4, 7, 3, 6, 1},
            {7, 6, 2, 3, 9, 1, 4, 5, 8},
            {3, 7, 1, 9, 5, 6, 8, 4, 2},
            {4, 9, 6, 1, 8, 2, 5, 7, 3},
            {2, 8, 5, 4, 7, 3, 9, 1, 6}
    };

    if (defFlag == 0) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                sudoku[i][j] = defSudoku[i][j];
            }
        }
    }

    // Print the Sudoku board
    printf("Validation Process for the Sudoku board:\n");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }

    pthread_t threads[11];
    int threadIndex = 0;
    int results[11];

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
    for (int i = 0; i < 11; i++) {
        void *result;
        pthread_join(threads[i], &result);
        results[i] = (int)(long)result;
    }

    // Check results
    int isValid = 1;
    for (int i = 0; i < 11; i++) {
        if (results[i] == 0) {
            isValid = 0;
            break;
        }
    }

    if (isValid) {
        printf("The Sudoku solution is valid.\n");
    } else {
        printf("The Sudoku solution is invalid.\n");
        for (int i = 0; i < 11; i++) {
            if (results[i] == 0) {
                switch (i) {
                    case 0:
                        printf("Invalid rows.\n");
                        break;
                    case 1:
                        printf("Invalid columns.\n");
                        break;
                    default:
                        printf("Invalid box %d.\n", i - 1);
                        continue;

                }
            }
        }
    }

    return 0;
}
