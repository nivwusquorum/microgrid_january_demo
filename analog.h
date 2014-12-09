/* 
 * File:   analog.h
 * Author: sidor
 *
 * Created on December 9, 2014, 5:12 PM
 */

#ifndef ANALOG_H
#define	ANALOG_H

#include <p32xxxx.h>

// must be called before read
void init_analog();

// reads value for pin
int analog_read(char analogPIN);



#endif	/* ANALOG_H */

