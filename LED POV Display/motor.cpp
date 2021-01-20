/*
 * CP_COP_MOTOR.c
 *
 * 2020-07-01
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define time 2000

int step_f[] = { 0x88, 0xCC, 0x44, 0x66, 0x22, 0x33, 0x11, 0x99 }; // step 상
unsigned char cnt;
int main(void)
{
    PORTF = 0x00;
    DDRF = 0xFF;
    sei();
    while (1) {
        for (cnt = 0; cnt < 8; cnt++) {
            PORTF = step_f[cnt]; // 정방향
            _delay_ms(4);
        }


    }