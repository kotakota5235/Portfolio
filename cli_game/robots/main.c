#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROW 60
#define COL 20
#define MAX_ROBOTS 40

typedef struct {
    int x;
    int y;
    int died;
} position;

extern char getChar(void);
int min(int a, int b);
void shuffle(int *array, int size);
void initialize(position *player, position robots[], int robots_num, int *array);
void print_board(position player, position robots[], int robots_num, int level, int score);
int action(position *player, position robots[], int robots_num);
void move_robots(position player, position robots[], int robots_num, int *score);
int check_collision(position player, position robots[], int robots_num);
void check_robots_collision(position robots[], int robots_num, int *score);
int check_level_clear(position robots[], int robots_num);


int main(void) { 
    int array[ROW*COL];
    position player;
    position robots[MAX_ROBOTS];
    int level = 1;
    int score = 0;
    int robots_num;

    for(int i = 0; i < ROW*COL; i++) {
        array[i] = i;
    }

    int flag = 1;
    while(flag) {
        shuffle(array, ROW*COL);
        robots_num = min(level * 5, MAX_ROBOTS);
        initialize(&player, robots, robots_num, array);
        print_board(player, robots, robots_num, level, score);

        while(1) {
            if(!action(&player, robots, robots_num)) {
                move_robots(player, robots, robots_num, &score);
            }
            print_board(player, robots, robots_num, level, score);
            if(check_collision(player, robots, robots_num) == 1) {
                printf("Game over... (level:%d score:%d)\n", level, score);
                flag = 0;
                break;
            }
            if(check_level_clear(robots, robots_num) == 0) {
                score += level * 10;
                level++;
                robots_num = min(level * 5, MAX_ROBOTS);
                break;
            }
        }
    }

    return 0;
}


int min(int a, int b) {
    if(a < b) return a;
    return b;
}


void shuffle(int *array, int size) {
    srand((unsigned)time(NULL));
    int j, temp;
    for(int i = size - 1; i > 0; i--) {
        j = rand() % (i + 1);
        temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}


void initialize(position *player, position robots[], int robots_num, int *array) {
    for(int i = 0; i < MAX_ROBOTS; i++) {
        robots[i].x = 0;
        robots[i].y = 0;
        robots[i].died = 0;
    }

    player -> x = array[0] / COL;
    player -> y = array[0] % COL;

    for(int i = 0; i < robots_num; i++) {
        robots[i].x = array[i + 1] / COL;
        robots[i].y = array[i + 1] % COL;
        robots[i].died = 0;
    }
}


void print_board(position player, position robots[], int robots_num, int level, int score) {
    char board[COL][ROW];

    for(int i = 0; i < COL; i++) {
        for(int j = 0; j < ROW; j++) {
            board[i][j] = ' ';
        }
    }
    //配置
    board[player.y][player.x] = '@';
    for(int i = 0; i < robots_num; i++) {
        if(robots[i].died) { 
            board[robots[i].y][robots[i].x] = '*';
        } else {
            board[robots[i].y][robots[i].x] = '+'; 
        }
    }
    //表示
    printf("\033[2J\033[1;1H");
    printf("+");
    for(int i = 0; i < ROW; i++) printf("-");
    printf("+\n");

    for(int i = 0; i < COL; i++) {
        printf("|");
        for(int j = 0; j < ROW; j++) {
            printf("%c", board[i][j]);
        }
        printf("|\n");
    }

    printf("+");
    for(int i = 0; i < ROW; i++) printf("-");
    printf("+\n");
    printf("(level:%d score:%d):?\n", level, score);
}


int action(position *player, position robots[], int robots_num) {
    char input;
    int x = player -> x; 
    int y = player -> y;

    while(1) {
        input = getChar();
        if((input - '0') >= 0 && (input - '0') <= 9) break;
    }

    switch(input) {
        case '0':
            srand((unsigned)time(NULL));
            x = rand() % ROW;
            y = rand() % COL;
            break;
        case '1':
            x--;
            y++;
            break;
        case '2':
            y++;
            break;
        case '3':
            x++;
            y++;
            break;
        case '4':
            x--;
            break;
        case '5':
            break;
        case '6':
            x++;
            break;
        case '7':
            x--;
            y--;
            break;
        case '8':
            y--;
            break;
        case '9':
            x++;
            y--;
            break;
    }

    //移動先がスクラップかどうか確認
    for(int i = 0; i < robots_num; i++) {
        if(robots[i].died && x == robots[i].x && y == robots[i].y) {
            return 1;
        }
    }
    //移動先が場外でなければ移動を実行
    if(x >= 0 && x < ROW && y >= 0 && y < COL) {
        player -> x = x;
        player -> y = y;
        return 0;
    }
    return 1;
}


void move_robots(position player, position robots[], int robots_num, int *score) {
    for(int i = 0; i < robots_num; i++) {
        if(robots[i].died) {
            continue;
        }

        if(robots[i].x < player.x) robots[i].x++;
        if(robots[i].x > player.x) robots[i].x--;
        if(robots[i].y < player.y) robots[i].y++;
        if(robots[i].y > player.y) robots[i].y--;
    }

    check_robots_collision(robots, robots_num, score);
}


int check_collision(position player, position robots[], int robots_num) {
    for(int i = 0; i < robots_num; i++) {
        if(robots[i].x == player.x && robots[i].y == player.y) { 
            return 1;
        }
    }
    return 0;
}


int compare_positions(const void *a, const void *b) {
    position *posA = (position *)a;
    position *posB = (position *)b;
    if(posA->x != posB->x) {
        return posA->x - posB->x;
    } else {
        return posA->y - posB->y;
    }
}

void check_robots_collision(position robots[], int robots_num, int *score) {
    qsort(robots, robots_num, sizeof(position), compare_positions);
    for(int i = 0; i < robots_num - 1; i++) {
        if(robots[i].x == robots[i + 1].x && robots[i].y == robots[i + 1].y) {
            if(!robots[i].died) *score += 1; 
            if(!robots[i + 1].died) *score += 1; 
            robots[i].died = 1;
            robots[i + 1].died = 1;
        }
    }
}


int check_level_clear(position robots[], int robots_num) {
    int died_robot = 0;
    for(int i = 0; i < robots_num; i++) {
        if(robots[i].died) died_robot++;
    }
    if(died_robot == robots_num) return 0; 
    return 1;
}

