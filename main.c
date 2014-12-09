#include <p32xxxx.h>
#include <stdint.h>
#include <plib.h>
//#include <delay.h>
//#include <util/delay.h>

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
// did something for 0 and 1
#define D5 PORTBbits.RB15
#define D6 PORTBbits.RB10
#define D7 PORTBbits.RB11
#define TRIS_D4 TRISBbits.TRISB14
#define TRIS_D5 TRISBbits.TRISB15
#define TRIS_D6 TRISBbits.TRISB10
#define TRIS_D7 TRISBbits.TRISB11

// PIC32MX250F128B Configuration Bit Settings
#pragma config FNOSC = FRCPLL, POSCMOD = OFF
#pragma config FPLLIDIV = DIV_2, FPLLMUL = MUL_20
#pragma config FPBDIV = DIV_1, FPLLODIV = DIV_2
#pragma config FWDTEN = OFF, JTAGEN = OFF, FSOSCEN = OFF
#define ARD_DEBUG 0

void LCD_Data(unsigned char data){
    delay_us(50); // Wait for previous data to finish
    RS = 1;
    // Send upper nibble first
    D0 = (data & 0x01);
    D1 = (data & 0x02) >> 1;
    D2 = (data & 0x04) >> 2;
    D3 = (data & 0x08) >> 3;
    D4 = (data & 0x10) >> 4;
    D5 = (data & 0x20) >> 5;
    D6 = (data & 0x40) >> 6;
    D7 = (data & 0x80) >> 7;
    EN = 1;
    delay_us(50);
    EN = 0;
    delay_us(50);
    // Lower nibble sent
    RS = 0;
}

void LCD_Char(unsigned char ch){
    LCD_Data(ch);
}

void LCD_Command(unsigned char command){
    delay_us(100); // Wait for previous command to finish
    RS = 0;
    RW = 0;
    delay_us(50);
    // Send upper nibble first
    D0 = (command & 0x01);
    D1 = (command & 0x02) >> 1;
    D2 = (command & 0x04) >> 2;
    D3 = (command & 0x08) >> 3;
    D4 = (command & 0x10) >> 4;
    D5 = (command & 0x20) >> 5;
    D6 = (command & 0x40) >> 6;
    D7 = (command & 0x80) >> 7;
    EN = 1;
    delay_us(50);
    EN = 0;
    delay_us(50);
    // Lower nibble sent
}

void LCD_InitWrite(unsigned char data){
    RS = 0;
    D4 = data & 0x01;
    D5 = (data & 0x02) >> 1;
    D6 = (data & 0x04) >> 2;
    D7 = (data & 0x08) >> 3;

}

void LCD_Init(void){
// Set all pins to output and clear them initially
    T1CON = 0x8030;
    TRIS_RS = 0; TRIS_EN = 0;
    TRIS_D0 = 0; TRIS_D1 = 0; TRIS_D2 = 0; TRIS_D3 = 0;
    TRIS_D4 = 0; TRIS_D5 = 0; TRIS_D6 = 0; TRIS_D7 = 0;
    RS = 0; EN = 0;
    D0 = 0; D1 = 0; D2 = 0; D3 = 0;
    D4 = 0; D5 = 0; D6 = 0; D7 = 0;
// Initialize sequence
    LCD_InitWrite(0x03);
    EN = 1;
    delay_ms(20);
    EN = 0;

    delay_us(250);

    LCD_InitWrite(0x03);
    EN = 1;
    delay_ms(7);
    EN = 0;

    delay_us(250);

    LCD_InitWrite(0x03);
    EN = 1;
    delay_us(500);
    EN = 0;

    delay_us(250);

    delay_us(250);


    LCD_Command(0x38);          // 4-bit 5x7
    LCD_Command(0x06);          // Increment cursor position; no display shift
    LCD_Command(0b00001111); // cursor should be on...
    LCD_Command(0x01);          // Clear display
    LCD_Command(0x80);          // Set DDRAM address to 0

}

void LCD_Clear(void){	//Clears LCD
    LCD_Command(1<<0);
}

void LCD_Home(void){                              //LCD cursor home
    LCD_Command(1<<1);
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

void main () {
    ANSELA = 0; // PORTA pins digital
    ANSELB = 0; // PORTB pins digital
    CM1CON = 0;
    CM2CON = 0;
    CM3CON = 0;
    DDPCONbits.JTAGEN = 0;
    //int count=0;
    T1CON =0x8030;
    TRISA=0x00;
    TRISB=0x00;
    PORTA=0x00;
    PORTB=0x00;
    LCD_Init();
    delay_ms(2000);
    LCD_print("waddup motherfucker");
    while (1) {
        //getNum(15);
        PORTAbits.RA4 = 1;
        delay_ms(500);
        PORTAbits.RA4 = 0;
        delay_ms(500);
    };
}

void LCD_print(char letters[]) {
    int count = 0;
    while (letters[count]){
        LCD_Char(letters[count]);
        LCD_Command(0x06);
        count++;
    };

}