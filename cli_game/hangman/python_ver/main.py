import sys, termios
import random

#参考：https://qiita.com/tortuepin/items/9ede6ca603ddc74f91ba
def getChar():
    # 標準入力のファイルディスクリプタ取得
    fd = sys.stdin.fileno()
    # 属性を取得 old は戻す用
    old = termios.tcgetattr(fd)
    new = old.copy()
    # 設定を変更
    new[3] &= ~termios.ICANON
    new[3] &= ~termios.ECHO
    try:
        # 設定を反映
        termios.tcsetattr(fd, termios.TCSANOW, new)
        ch = sys.stdin.read(1)
    finally:
        # 設定を元に戻す
        termios.tcsetattr(fd, termios.TCSANOW, old)
    # 文字表示
    return ch

def get_random_words():
    array = []
    f = open('toeic1500_utf.dat', 'r')
    for line in f:
        array.append(line.split(' ')[1])
    random.shuffle(array)
    f.close()
    return array[0]

def disp_info(guess, used_char, wrong_count):
    print("単語：", ''.join(guess))
    print("使われた文字：", *used_char)
    print("残り回数：", (7 - wrong_count))
    print("文字を入力してください：\n")

question = get_random_words()
question_len = len(question)
guess = []
used_char = []
wrong_count = 0
for i in range(question_len):
    guess+='-'
print(question)

flag = True
while flag:
    disp_info(guess, used_char, wrong_count)
    input_char = getChar()
    while 1:
        if input_char in used_char:
            print("この文字はすでに使われています")
            disp_info(guess, used_char, wrong_count)
            input_char = getChar()
        elif input_char < 'a' or input_char > 'z':
            input_char = getChar()
        else:
            break

    correct = False
    for i in range(question_len):
        if input_char == question[i]:
            guess[i] = question[i]
            if ''.join(used_char).find(input_char) == -1:
                used_char.append(input_char)
            correct = True
            if ''.join(guess) == question:
                print(''.join(guess))
                print("ゲームクリア")
                flag = False
    if not correct:
        used_char.append(input_char)
        wrong_count+=1
        correct = False
        if wrong_count > 6:
            print("答え：", question)
            print("ゲームオーバー")
            flag = False

