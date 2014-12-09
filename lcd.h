/* 
 * File:   lcd.h
 * Author: sidor
 *
 * Created on December 9, 2014, 12:10 AM
 */

#ifndef LCD_H
#define	LCD_H

#include <p32xxxx.h>

#include "timer.h"

#define RW PORTBbits.RB8
#define RS PORTBbits.RB7
#define EN PORTBbits.RB9

#define D0 PORTAbits.RA0
#define D1 PORTAbits.RA1
#define D2 PORTAbits.RA2
#define D3 PORTAbits.RA3

#define D4 PORTBbits.RB14
#define D5 PORTBbits.RB15
#define D6 PORTBbits.RB10
#define D7 PORTBbits.RB11

// Must be called before everything else
// Takes a long time! (about 2s).
void LCD_init();

// write a single character to screen
void LCD_char(unsigned char ch);

// clear display
void LCD_clear();

// move cursor to beginning of screen (lop-left)
void LCD_home();

// print a string
void LCD_print(char letters[], int line);

#endif	/* LCD_H */

