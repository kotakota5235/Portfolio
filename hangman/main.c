#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <termio.h>
#include <ctype.h>

/*単語の最大長*/
#define MAX 50

char getChar(void);
char* tolower_str(char* s);
void disp_info(char *guess, char* used_char, int wrong_count);

int main(int argc, char *argv[]) {
  /*単語データ読み込み*/
  /*引数なしであればwords、引数で指定があればそのファイルを読み込む*/
  FILE *fp = NULL;
  if(argc == 1) {
    fp = fopen("words", "r");
  }
  else if (argc == 2) {
    fp = fopen(argv[1], "r");
  }
  else {
    fprintf(stderr, "Usage : %s <Words file name>\n", argv[0]);
    return 1;
  }

  if(fp == NULL) {
    fprintf(stderr, "%s is not found.\n", argv[1]);
    return 1;
  }
  /*読み込むファイルの単語数（行数）の把握*/
  int file_length = 0;
  char question[MAX];
  while(fgets(question, MAX, fp) != NULL) {
    file_length++;
  }

  /*変数宣言*/
  char* guess;
  char used_char[26];
  int used_char_index = 0;
  char input;
  int wrong = 0, true = 0, found = 0, flag = 1;
  int q_no, q_len;

  /*ゲーム部分*/
  while(1) {
   /*問題をランダムに選択*/
    srand((unsigned)time(NULL));
    int q_no = rand() % file_length;
    rewind(fp);
    for(int i = 0; i < q_no; i++) {
      fgets(question, MAX, fp);
    }
    /*改行文字を終端文字に置き換え*/
    char* temp = strchr(question, '\n');
    if (temp != NULL) {
      *temp = '\0';
    }
    tolower_str(question);
    q_len = strlen(question);

    /*推測用の配列を動的に確保*/
    guess = (char *)malloc(strlen(question));
    for(int i = 0; i < q_len; i++) {
      guess[i] = '-';
    }
    guess[q_len] = '\0';

    /*推測と判定*/
    while(flag) {
      true = 0, found = 0;
      /*状態を表示*/
      disp_info(guess, used_char, wrong);

      /*当てる文字を入力させる*/
      /*英小文字だけ受け付ける*/
      do {
        input = getChar();
      }
      while(input < 'a' || input > 'z'); 

      printf("\n\n");
      /*すでに入力されているか判定*/
      for(int i = 0; i < 26; i++) {
        if(input == used_char[i]) {
          printf("この文字はすでに使われています\n\n");
          flag = 0;
        }
      }
      if(!flag) {
        flag = 1;
        continue;
      }

      /*判定*/
      for(int j = 0; j < q_len; j++) {
        /*合っていたら*/
        if(input == question[j]) {
          /*推測用配列に代入*/
          guess[j] = question[j];

          /*既入力の配列に代入*/
          for(int i = 0; i < 26; i++) {
            if(input == used_char[i]) {
              found = 1;
              break;
            }
          }
          if(!found) {
            used_char[used_char_index++] = input;
          }

          /*クリア判定*/
          if(strcmp(question, guess) == 0) {
            printf("%s\n", guess);
            printf("ゲームクリア\n");
            flag = 0;
            free(guess);
          } 
          true++;
        } 
      }

      /*間違っていたら*/
      if(!true) {
        used_char[used_char_index++] = input;
        wrong++;
        true = 0;
        /*ゲームオーバー判定*/
        if(wrong > 6) {
          flag = 0;
          wrong = 0;
          printf("答え : %s\n", question);
          printf("ゲームオーバー\n");
          free(guess);
        }
      }
    }

    /*繰り返しプレイの確認*/
    printf("続けますか？(y/n)：");
    input = getChar();
    printf("\n");
    char option = toupper(input);
    switch(option)
    {
      case 'Y':
        flag = 1;
        for(int i = 0; i < 26; i++) {
          used_char[i] = '\0';
        }
        used_char_index = 0; 
        continue;

      case 'N':
        printf("\nbye\n");
        return 0;

      default:
        printf("\nInvalid option.\n");
        flag = 0;
        continue;
    }
  }

  fclose(fp);
  return 0;
}


char getChar(void) {
  struct termio old_term, new_term;
  char c;
  /* 現在の設定を得る */
  ioctl(0, TCGETA, &old_term);
  /* 設定のコピーをつくる */
  new_term = old_term;
  /* 入力文字のエコーを抑止する場合 */
  new_term.c_lflag &= ~(ICANON | ECHO);
  /* エコーは止めない場合 */
  //new_term.c_lflag &= ~(ICANON);
  /* 新しい設定を反映する */
  ioctl(0, TCSETAW, &new_term);
  /* 1 文字入力 */
  c = getchar();
  /* 古い設定に戻す */
  ioctl(0, TCSETAW, &old_term);
  return (c);
}

char* tolower_str(char* s)
{
  for (char* p = s; *p != '\0'; ++p) {
    *p = tolower(*p);
  }
  return s;
}

void disp_info(char* guess, char* used_char, int wrong_count) {
  printf("単語 : %s\n", guess);
  printf("使われた文字 : ");
  printf("%s\n", used_char);
  printf("残り回数 : %d\n", (7 - wrong_count));
  printf("文字を入力してください : ");
}

