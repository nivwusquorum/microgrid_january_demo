#include "timer.h"

#include <p32xxxx.h>

void timer_init(void) {
    T1CON = 0x8030;
}

int delay_ms(int x) {
    while (x>0) {
        delay_1ms();
        x=x-1;
    }
}

int delay_us(int x) {
    int y = x/50;
    while (y>0) {
        delay_50us();
        y = y-1;
    }
}

int delay_50us() {
    TMR1=0; while(TMR1<50/6);
    return 1;
}
int delay_1ms() {
    int x = 20;
    while (x>0) {
        delay_50us();
        x=x-1;
    }
}
