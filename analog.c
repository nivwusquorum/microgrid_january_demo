#include "analog.h"


int analog_read(char analogPIN){
    AD1CHS = analogPIN << 16;       // AD1CHS<16:19> controls which analog pin goes to the ADC

    AD1CON1bits.SAMP = 1;           // Begin sampling
    while( AD1CON1bits.SAMP );      // wait until acquisition is done
    while( ! AD1CON1bits.DONE );    // wait until conversion done

    return ADC1BUF0;                // result stored in ADC1BUF0
}

void init_analog(){
    AD1CON1CLR = 0x8000;    // disable ADC before configuration

    AD1CON1 = 0x00E0;       // internal counter ends sampling and starts conversion (auto-convert), manual sample
    AD1CON2 = 0;            // AD1CON2<15:13> set voltage reference to pins AVSS/AVDD
    AD1CON3 = 0x0f01;       // TAD = 4*TPB, acquisition time = 15*TAD
}