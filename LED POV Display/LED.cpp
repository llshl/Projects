#include <stdio.h>
#include <string.h>
#include <Windows.h>
char num_total[10] = "        ";
//char* make_total0 =NULL;
char* mode = "[123]          [A]";
static char make_total[10] = "";
char rx_mode;
int receive = 0;
char* makeword(char data) {
    static int i = 0;
    if (data == '[') {
        for (i = 0; i < 10; i++) {
            make_total[i] = NULL;
        }
        i = 0;
    }

    while (1) {
        printf("문자열 %d", i);
        make_total[i++] = data;
        if (data == ']') {
            receive = 0;
            break;
        }
        break;
    }
    return make_total;
}
int player(char   data) {
    int i = 0;
    static int d = 0;
    if (data >= 48 && data <= 57) {
        while (!(i == 9)) {
            printf("%c출력", data);
            i++;
        }
    }
    else if (data >= 65 && data <= 90) {
        while (!(i == 9)) {
            printf("%c출력", data);
            i++;
        }
    }
    return data;
}
int playerword(char startword) {
    int i;
    int arraysize = strlen(make_total);
    if (startword == '[') {
        for (i = 1; i < (arraysize - 1); i++) {
            player(make_total[i]);
        }
    }
    return startword;
}
void main() {
    int pointer = 0;
    while (1) {
        rx_mode = mode[pointer++];
        if (rx_mode == '[') {
            receive = 1;
        }
        if (receive == 0) {
            playerword(make_total[0]);
        }
        if (receive == 1) {
            makeword(rx_mode);
        }
        Sleep(500);
    }
}