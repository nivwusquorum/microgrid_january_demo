/* 
 * File:   timer.h
 * Author: sidor
 *
 * Created on December 9, 2014, 12:25 AM
 */

#ifndef TIMER_H
#define	TIMER_H

// must be called before all the other timer functions.
void timer_init(void);

// Delay X milliseconds.
int delay_ms(int x);

// Delay 1 millisecond.
int delay_1ms();

// Delay x microseconds. X should be multiple of 50
int delay_us(int x);

// Delay 50 microseconds
int delay_50us();




#endif	/* TIMER_H */

