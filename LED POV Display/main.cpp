#include "LED.h"
#define delay1   10
#define delay2   6
#define delay3   5
#define delay4   4
#define delay5   3
#define delay6   2
#define delaynull 20
char num_total[10];
static char make_total[10] = "";
char rx_mode;
static int point;
int LEDwrite(int i) {
    PORTA = num_total[i];
    if (strlen(make_total) == 3) {
        _delay_ms(delay1);
    }
    else if (strlen(make_total) == 4) {
        _delay_ms(delay2);
    }
    else if (strlen(make_total) == 5) {
        _delay_ms(delay3);
    }
    else if (strlen(make_total) == 6) {
        _delay_ms(delay4);
    }
    else if (strlen(make_total) == 7) {
        _delay_ms(delay5);
    }
    else _delay_ms(delay6);
    return i;
}
void LED_init() {
    UCSR0A = 0;
    UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);        //RX INT enable, RX & TX Enable
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);                    //8bit no parity
    UBRR0H = 0;
    UBRR0L = 51;                                        //19200 baudrate
}
char* makeword(char data) {
    static int i = 0;
    if (data == '[') {
        PORTA = 0x02;
        for (i = 0; i < 10; i++) {
            make_total[i] = NULL;
        }
        i = 0;
    }
    PORTA = 0x00;
    PORTA = 0x01;
    make_total[i++] = data;
    return make_total;
}
ISR(USART0_RX_vect)
{
    rx_mode = UDR0;
    point = 1;   //입력신호 들어올 때 1
    UCSR0B |= (1 << UDRIE0);
}
ISR(USART0_UDRE_vect)
{
    UDR0 = rx_mode;
    UCSR0B &= ~(1 << UDRIE0);
    _delay_ms(50);
}
int player(char  data, int del) {
    int i = 0;
    static int d = 0;
    d = del;
    if (data >= 48 && data <= 57) {
        while (!(i == (9 - d))) {
            num_total[i] = num_0[((data - 48) * 10) + i + d];
            LEDwrite(i);
            i++;
        }
    }
    else if (data >= 65 && data <= 90) {
        while (!(i == (9 - d))) {
            num_total[i] = num_A[((data - 65) * 10) + i + d];
            LEDwrite(i);
            i++;
        }
    }
    return data;
}
int playerword(char startword) {
    int i;
    int i2;
    int arraysize = strlen(make_total);
    if (startword == '[') {
        for (i = 0; i < delaynull; i++) {
            PORTA = num_NULL[i];
            PORTB = num_NULL[i];
            _delay_ms(delay3);
        }
        for (i = 1; i < (arraysize - 1); i++) {
            if (i == 1) {
                player(make_total[i], 0);
            }
            else if (i > 1) {
                player(make_total[i], 1);
            }
        }
    }
    return startword;
}
void main() {
    DDRA = 0xff;
    DDRB = 0xff;
    PORTA = 0x00;
    PORTB = 0x00;
    LED_init();
    sei();
    static int receive;
    while (1) {
        while (!(UCSR0A & 0x20)); // 송신데이터를 받을 준비가 될때까지 대기
        if ((make_total[0] == '[') && (make_total[strlen(make_total) - 1] != ']')) {
            receive = 1;
        }
        else if (rx_mode == ']') {
            receive = 0;
        }
        if (receive == 0) {   //문자열 입력중일 때를 제외하고 모든 상황에서 led 출력
            if (rx_mode == '*') {
                make_total[10] = "";
            }
            PORTB = 0x01;
            playerword(make_total[0]);
        }
        if (point == 1) {
            PORTB = 0x02;
            makeword(rx_mode);
            point = 0;   //바로 0으로 만듬으로써 입력한 값이 한번만 문자열에 추가되도록 실행
        }
    }
}