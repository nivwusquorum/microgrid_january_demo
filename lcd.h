/*
 * File:   lcd.h
 * Author: sidor
 *
 * Created on December 9, 2014, 12:10 AM
 */

#ifndef LCD_H
#define	LCD_H

#include <p33EP512GM710.h>

#include "timer.h"

/* WIRING
 * If we look on the board from the top such that MIT logo is in top-left:
 * D7 D6
 * D5 D4
 * D3 D2
 * D1 D0
 * E R/W
 * RS  +
 *  +  -
 *
 * WARNING: Remeber that LCD requires two negative (one of which is V0 for
 * contrast) and one positive supplies. What you have available on board is
 * not idead (two positives and one negative, so you need to be a little bit
 * creative.
 * 
 * Tack time!
 */

#define RS LATEbits.LATE15
#define RW LATDbits.LATD5
#define EN LATCbits.LATC8

#define D0 LATBbits.LATB10
#define D1 LATBbits.LATB11
#define D2 LATBbits.LATB12
#define D3 LATBbits.LATB13

#define D4 LATAbits.LATA10
#define D5 LATAbits.LATA7
#define D6 LATBbits.LATB14
#define D7 LATBbits.LATB15

// Must be called before everything else
// Takes a long time! (about 2s).
void LCD_init(void);

// write a single character to screen
void LCD_char(unsigned char ch);

// clear display
// a bit slow (10ms).
void LCD_reset(void);

// move cursor to beginning of line.
void LCD_home(int line);

// moves cursor to right(1) or left(0).
void LCD_move_cursor(int right);

// print a string
void LCD_print(char letters[], int line);

// if depending on "on" value ensures cursor is on or off
// notice that cursor might be behind your current line
void LCD_cursor(int on);

// Output a number at current cursor position.
void LCD_int(unsigned int number);

#endif	/* LCD_H */
