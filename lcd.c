#include "lcd.h"

#include "timer.h"

#define REG volatile unsigned char


void LCD_init(){

    // Set all pins to output and clear them initially
    RW = 0; RS = 0; EN = 0;
    D0 = 0; D1 = 0; D2 = 0; D3 = 0;
    D4 = 0; D5 = 0; D6 = 0; D7 = 0;

    int repeat = 3;
    while(repeat--) {
        // Initialize sequence
        LCD_write_info(0x30);
        EN = 1;
        delay_ms(1);
        EN = 0;
    }

    delay_us(500);

    LCD_command(0x38);          // 4-bit 5x7
    LCD_command(0x06);          // Increment cursor position; no display shift
    LCD_command(0b00001111);    // cursor should be on...
    LCD_command(0x01);          // Clear display
    LCD_command(0x80);          // Set DDRAM address to 0

    delay_ms(2000);
}

void LCD_data(unsigned char data){
    delay_us(50); // Wait for previous data to finish
    RS = 1;
    LCD_write_info(data);
    EN = 1;
    delay_us(50);
    EN = 0;
    delay_us(50);
    // Lower nibble sent
    RS = 0;
}

void LCD_command(unsigned char command){
    delay_us(100); // Wait for previous command to finish
    RS = 0;
    RW = 0;
    delay_us(50);
    LCD_write_info(command);
    EN = 1;
    delay_us(50);
    EN = 0;
    delay_us(50);
    // Lower nibble sent
}



void LCD_char(unsigned char ch){
    LCD_data(ch);
}

void LCD_write_info(unsigned char info) {
    D0 = (info & 0x01);
    D1 = (info & 0x02) >> 1;
    D2 = (info & 0x04) >> 2;
    D3 = (info & 0x08) >> 3;
    D4 = (info & 0x10) >> 4;
    D5 = (info & 0x20) >> 5;
    D6 = (info & 0x40) >> 6;
    D7 = (info & 0x80) >> 7;
}


void LCD_clear(){	//Clears LCD
    LCD_command(1<<0);
}

void LCD_home(){                              //LCD cursor home
    LCD_command(1<<1);
}

void LCD_print(char letters[], int line) {
    // set cursor to appropriate position.
    int count = 0;
    while (letters[count] && count < 16){
        LCD_char(letters[count]);
        LCD_command(0x06);
        count++;
    };
}

