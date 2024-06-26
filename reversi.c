#include <stdio.h>
#include <stdlib.h>
#define SIZE 6

char board[SIZE][SIZE] = {0};
_Bool valid[SIZE][SIZE] = {0};
const char status[3] = {'#', 'O', 'X'};
char yx[2] = {0};

void updateValid(char i, char j) {
    for (char k = i-1; k <= i+1; k++) {
        for (char l = j-1; l <= j+1; l++) {
            if (k >= 0 && l >= 0 && k < SIZE && l < SIZE) {
                if (board[k][l] == 0) {
                    valid[k][l] = 1;
                }
            }
        }
    }
}

void _totalUpdateValid(void) {
    for (char i = 0; i < SIZE; i++) {
        for (char j = 0; j < SIZE; j++) {
            if (board[i][j] > 0) {
                updateValid(i, j);
            }
        }
    }
}

void _displayValid(void) {
    printf("\n");
    for (char i = 0; i < SIZE; i++) {
        for (char j = 0; j < SIZE; j++) {
            printf("%d ", valid[i][j]);
        }
        printf("\n");
    }
}

char _furthest(char y, char x, char id, char a, char b) {
    char k = 1;
    char furthest = 0;
    while (y+k*b >= 0 && y+k*b < SIZE &&
           x+k*a >= 0 && x+k*a < SIZE &&
           board[y+k*b][x+k*a] > 0) {

        if (board[y+k*b][x+k*a] == id) {
            furthest = k;
        }
        k++;
    }

    return furthest;
}

void _flip(char y, char x, char id, char a, char b) {
    char furthest = _furthest(y, x, id, a, b);

    for (char i=1; i < furthest; i++) {
        printf("flipping (%d, %d)\n", x+i*a, y+i*b);
        board[y+i*b][x+i*a] = id;
    }
}

void flip(char y, char x, char id) {
    board[y][x] = id;
    valid[y][x] = 0;

    for (char i = -1; i <= 1; i++) {
        for (char j = -1; j <= 1; j++) {
            if ((i != 0) || (j != 0)) {
                _flip(y, x, id, i, j);
            }
        }
    }
}

char _eval(char y, char x, char id, char a, char b) {
    char furthest = _furthest(y, x, id, a, b);

    char i = 1;
    char score = 0;

    while(i < furthest) {
        if (board[y+i*b][x+i*a] == 3-id) {
            score++;
        }
        i++;
    }

    return score;
}

char eval(char y, char x, char id) {
    char score = 0;
    for (char i = -1; i <= 1; i++) {
        for (char j = -1; j <= 1; j++) {
            if (i != 0 || j != 0) {
                score = score + _eval(y, x, id, i, j);
            }
        }
    }
    return score;
}

void _displayEvalBy(char id) {
    printf("\n");
    for (char i = 0; i < SIZE; i++) {
        for (char j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                printf("%d ", eval(i, j, id));
            } else {
                printf("0 ");
            }
        }
        printf("\n");
    }
}

void displayBoard(void) {
    printf("\e[1;1\e[2J")
    printf("\n  0 1 2 3 4 5\n");
    for (char i = 0; i < SIZE; i++) {
        printf("%d ", i);
        for (char j = 0; j < SIZE; j++) {
            printf("%c ", status[board[i][j]]);
        }
        printf("\n");
    }
}

void updateBoard(char id) {
    flip(yx[0], yx[1], id);
    updateValid(yx[0], yx[1]);
}

void resetBoard(void) {
    board[2][2] = 1;
    board[2][3] = 2;
    board[3][2] = 2;
    board[3][3] = 1;
    _totalUpdateValid();
}

void playerMove(void) {
    for (;;) {
        short x, y;
        printf("\nEnter the x coordinate: ");
        scanf("%hd", &x);
        printf("Enter the y coordinate: ");
        scanf("%hd", &y);

        if (valid[y][x] == 1) {
            printf("\nYou played (%d, %d)\n", x, y);
            yx[0] = y;
            yx[1] = x;
            break;
        } else {
            printf("\nInvalid move!\n");
            displayBoard();
        }
    }
}

void compMove(void) {
    char score, bestX, bestY;
    char bestScore = -1;
 
    for (char i = 0; i < SIZE; i++) {
        for (char j = 0; j < SIZE; j++) {
            if (valid[i][j] == 1) {
                score = eval(i, j, 2);
                if (score > bestScore) {
                    bestScore = score;
                    bestX = j;
                    bestY = i;
                } else if (score == bestScore) {
                    if (rand()%101 > 50) {
                        bestX = j;
                        bestY = i;
                    }
                }

            }
        }
    }
    printf("\nComputer played (%d, %d)\n", bestX, bestY);
    yx[0] = bestY;
    yx[1] = bestX;
}

char countScore(void) {
    char score = 0;
    for (char i = 0; i < SIZE; i++) {
        for (char j = 0; j < SIZE; j++) {
            if (board[i][j] == 1) {
                score++;
            }
        }
    }
    return score;
}

void main(void) {
    resetBoard();
    printf("\nGame Start!\n");
    displayBoard();

    for (char i = 4; i < SIZE*SIZE;) {
        playerMove();
        updateBoard(1);
        i++;
        compMove();
        updateBoard(2);
        i++;
        displayBoard();
    }

    char score = countScore();
    char threshold = SIZE*SIZE/2;

    printf("You scored %d/%d", score, SIZE*SIZE);

    if (score > threshold) {
        printf("\nYou have won!\n");
    } else if (score == threshold) {
        printf("\nIt's a draw!\n");
    } else {
        printf("\nYou have lost!\n");
    }
}
