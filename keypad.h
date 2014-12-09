/* 
 * File:   keypad.h
 * Author: sidor
 *
 * Created on December 9, 2014, 5:27 PM
 */

#ifndef KEYPAD_H
#define	KEYPAD_H

#include "timer.h"
#include "analog.h"
#include "lcd.h"

#define NUM_KEYS 12
#define ANALOG_PIN 11

int Keypad_get_key(void);

void Keypad_init_static(void);

void Keypad_calibrate(int eeprom_address);

char Keypad_char_for_key(int key);

#endif	/* KEYPAD_H */

