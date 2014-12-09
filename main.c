#include <p32xxxx.h>
#include <stdint.h>
#include <plib.h>

#include "timer.h"
#include "lcd.h"
#include "analog.h"

#define RW PORTBbits.RB8
#define RS PORTBbits.RB7
#define EN PORTBbits.RB9
#define TRIS_RS TRISBbits.TRISB7
#define TRIS_RW TRISBbits.TRISB8
#define TRIS_EN TRISBbits.TRISB9

#define D0 PORTAbits.RA0
#define D1 PORTAbits.RA1
#define D2 PORTAbits.RA2
#define D3 PORTAbits.RA3
#define TRIS_D0 TRISAbits.TRISA0
#define TRIS_D1 TRISAbits.TRISA1
#define TRIS_D2 TRISAbits.TRISA2
#define TRIS_D3 TRISAbits.TRISA3

#define D4 PORTBbits.RB14
#define D5 PORTBbits.RB15
#define D6 PORTBbits.RB10
#define D7 PORTBbits.RB11
#define TRIS_D4 TRISBbits.TRISB14
#define TRIS_D5 TRISBbits.TRISB15
#define TRIS_D6 TRISBbits.TRISB10
#define TRIS_D7 TRISBbits.TRISB11

#define LED PORTAbits.RA4
#define KEYPAD PORTBbits.RB13

// PIC32MX250F128B Configuration Bit Settings
#pragma config FNOSC = FRCPLL, POSCMOD = OFF
#pragma config FPLLIDIV = DIV_2, FPLLMUL = MUL_20
#pragma config FPBDIV = DIV_1, FPLLODIV = DIV_2
#pragma config FWDTEN = OFF, JTAGEN = OFF, FSOSCEN = OFF


void pin_init(void) {
    // Make sure all the LCD pins are recognized as digital
    TRIS_RS = 0; TRIS_EN = 0;
    TRIS_D0 = 0; TRIS_D1 = 0; TRIS_D2 = 0; TRIS_D3 = 0;
    TRIS_D4 = 0; TRIS_D5 = 0; TRIS_D6 = 0; TRIS_D7 = 0;

    ANSELA = 0; // PORTA pins digital
    ANSELB = 0; // PORTB pins digital
    CM1CON = 0;
    CM2CON = 0;
    CM3CON = 0;
    DDPCONbits.JTAGEN = 0;
    //int count=0;
    TRISA=0x00;
    TRISB=0x00;
    PORTA=0x00;
    PORTB=0x00;

    // keypad setup
    ANSELBbits.ANSB13 = 1;   // set RB3 (AN5) to analog
    TRISBbits.TRISB13 = 1;   // set RB3 as an input
    init_analog();
    AD1CON1SET = 0x8000;    // Enable ADC
}


void init(void) {
    pin_init();
    timer_init();
    LCD_init();
}

void main () {
    init();
    LCD_print("NOT SAMPLED", 0); // top line

    LED = 1;
    while (1) {
        //getNum(15);
        LED = 0;
        delay_ms(300);
        LED = 1;
        delay_ms(300);

        LCD_print("READ:", 0);
        LCD_int(analog_read(11));
    };
}