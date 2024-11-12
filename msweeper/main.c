#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void put_M(int board[8][8]);
void put_num(int board[8][8]);
void start_disp(char disp_board[8][8]);

void game();
void disp(int x, int y, char disp_board[8][8]);
char input(int *x, int *y);
void mark_m(int x, int y, int board[8][8], char disp_board[8][8]);
void mark_question(int x, int y, int board[8][8], char disp_board[8][8]);
int open(int x, int y, int board[8][8], char disp_board[8][8]);
int check_clear(int board[8][8]);

int main(void) {
    game();
    return 0;
}

void game() { 
    int x, y, flag = 0; 
    char mode;
    int board[8][8];
    char disp_board[8][8];
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            board[i][j] = 0;
        }
    }
    put_M(board);
    put_num(board);
    start_disp(disp_board);
    while(!flag) {
        mode = input(&x, &y);
        if(mode == 'm') {
            mark_m(y, x, board, disp_board);
            disp(y, x, disp_board);
        } else if(mode == 'q') {
            mark_question(y, x, board, disp_board);
            disp(y, x, disp_board);
        } else {
            if(open(y, x, board, disp_board) == 1) {
                printf("Mを踏んだ\n");
                break;
            } else {
                disp(y, x, disp_board);
            }
            if(check_clear(board) == 0){
                //クリア
                flag = 1;
                printf("クリア\n");
            } else {
                continue;
            }
        }
    }
}

void disp(int x, int y,  char disp_board[8][8]) {
    printf("  0 1 2 3 4 5 6 7\n"); 
    for(int i = 0; i < 8; i++) {
        printf("%d ", i);
        for(int j = 0; j < 8; j++) {
            printf("%c ", disp_board[i][j]);
        }
        printf("\n");
    }
}

char input(int *x, int *y) {
    char mode;
    while(1){
        printf("Please input (x y mode) : ");
        scanf("%d %d %c", x, y, &mode);
        if(*x < 0 || *x > 7 || *y < 0 || *y > 7 || (mode != 's' && mode != 'm' && mode != 'q')) {
            printf("Input error. Please input again.\n");
        } else {
            break;
        }
    }
    return mode;
}

int open(int x, int y, int board[8][8], char disp_board[8][8]) {
    //開いた場所は-2とする
    if(board[x][y] == -1) {
        //gameover
        return 1;
    } else if(board[x][y] == 0) {
        board[x][y] = -2;
        disp_board[x][y] = ' ';
    } else if(board[x][y] == -2) {
    } else {
        disp_board[x][y] = '0' + board[x][y];
        board[x][y] = -2;
    }
    return 0;
}

void mark_m(int x, int y, int board[8][8], char disp_board[8][8]) { 
    if(disp_board[x][y] == 'M') {
        disp_board[x][y] = '.';
    } else if(board[x][y] == -2) {
    } else {
        disp_board[x][y] = 'M';
    }
}

void mark_question(int x, int y, int board[8][8], char disp_board[8][8]) {
    if(disp_board[x][y] == '?') {
        disp_board[x][y] = '.';
    } else if(board[x][y] == -2) {
    } else {
        disp_board[x][y] = '?';
    }
}

int check_clear(int board[8][8]) {
    int flag = 0;
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(board[j][i] >= 0) {
                flag = 1;
                break;
            }
        }
        if(flag) return 1;
    }
    return 0; 
}

void start_disp(char disp_board[8][8]) {
    printf("  0 1 2 3 4 5 6 7\n");
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            disp_board[i][j] = '.';
        }
    }
    for(int i = 0; i < 8; i++) {
        printf("%d ", i);
        for(int j = 0; j < 8; j++) {
            printf("%c ", disp_board[i][j]);
        }
        printf("\n");
    }
}

void put_M(int board[8][8]) {
    srand((unsigned)time(NULL));
    int row, column;
    for(int i = 0; i < 10; i++) {
        do {
            row = rand() % 8;
            column = rand() % 8;
        } while(board[row][column] == -1); 
        board[row][column] = -1;
    }
}

void put_num(int board[8][8]) {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            //(i,j)がMだったときのスキップ処理
            if(board[i][j] == -1) {
                continue;
            }
            //(i,j)の周りのMの数をカウントし、(i,j)をインクリメント
            for(int k = -1; k <= 1; k++) {
                for(int l = -1; l <= 1; l++) {
                    if(i+k < 0 || i+k > 7 || j+l < 0 || j+l > 7){
                        continue;
                    }
                    if(board[i+k][j+l] == -1) {
                        board[i][j] += 1;
                    }
                }
            }
        }
    }
}

