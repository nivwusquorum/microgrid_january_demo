/*
 * File:   main.c
 * Author: Dan
 *
 * Created on November 24, 2014, 12:59 PM
 * Code for dsPIC33E, comm board
 */

#include <p33EP512GM710.h>

#include "lcd.h"
#include "NodalCommunication.h"
#include "send_to_arduino.h"
#include "SPI_POWER_BOARD.h"
#include "SPI_EEPROM.h"
#include "timer.h"


/* How big are different types?
 *
 * short 2
 * int 2
 * long 4
 * long long 8
 */

#define TRIS_INPUT 1
#define TRIS_OUTPUT 0

// send_to_arduino
#define TRIS_AUX TRISAbits.TRISA0
#define TRIS_AUX2 TRISAbits.TRISA11
#define TRIS_AUX3 TRISAbits.TRISA12

#define TRIS_RS TRISEbits.TRISE15
#define TRIS_RW TRISDbits.TRISD5
#define TRIS_EN TRISCbits.TRISC8

#define TRIS_D0 TRISBbits.TRISB10
#define TRIS_D1 TRISBbits.TRISB11
#define TRIS_D2 TRISBbits.TRISB12
#define TRIS_D3 TRISBbits.TRISB13

#define TRIS_D4 TRISAbits.TRISA10
#define TRIS_D5 TRISAbits.TRISA7
#define TRIS_D6 TRISBbits.TRISB14
#define TRIS_D7 TRISBbits.TRISB15

#define WREN2    0x06    // write enable
#define RDSR2    0x05    // read STATUS register
#define SS2_2      LATCbits.LATC10

// Internal FRC Oscillator
_FOSCSEL(FNOSC_FRCPLL);  // FRC Oscillator
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF  & POSCMD_NONE);   // Clock Switching is enabled and Fail Safe Clock Monitor is disabled
						     // OSC2 Pin Function: OSC2 is Clock Output
                                                     // Primary Oscillator Mode: Disabled
_FWDT(FWDTEN_OFF);              // Watchdog Timer Enabled/disabled by user software
_FGS(GWRP_OFF); // General segment Write protect turned off
_FICD(ICS_PGD2 & JTAGEN_OFF) // Select Debug/Program Lines and JTAG OFF


void initOscillator(){
  // setup internal clock for 70MHz/35MIPS
  // 7.37/2=3.685*43=158.455/2=79.2275
  CLKDIVbits.PLLPRE=0;        // PLLPRE (N2) 0=/2
  //PLLFBD=41;                  // pll multiplier (M) = +2
  //PLLFBD = 41;  //PUT BACK TO 41!

  PLLFBD = 35;  //PUT BACK TO 41
  CLKDIVbits.PLLPOST=0;       // PLLPOST (N1) 0=/2

  RCONbits.SWDTEN = 0;
}

void init_pins(void) {
    TRIS_AUX = TRIS_OUTPUT;
    TRIS_AUX2 = TRIS_OUTPUT;
    TRIS_AUX3 = TRIS_OUTPUT;

    TRIS_RS = TRIS_OUTPUT;
    TRIS_RW = TRIS_OUTPUT;
    TRIS_EN = TRIS_OUTPUT;
    TRIS_D0 = TRIS_OUTPUT;
    TRIS_D1 = TRIS_OUTPUT;
    TRIS_D2 = TRIS_OUTPUT;
    TRIS_D3 = TRIS_OUTPUT;
    TRIS_D4 = TRIS_OUTPUT;
    TRIS_D5 = TRIS_OUTPUT;
    TRIS_D6 = TRIS_OUTPUT;
    TRIS_D7 = TRIS_OUTPUT;

    // All pins digital for starts.
    ANSELA = 0; // PORTA pins digital
    ANSELB = 0; // PORTB pins digital
    ANSELC = 0; // PORTC pins digital
    ANSELD = 0; // PORTD pins digital
    ANSELE = 0; // PORTE pins digital
}



void init(void) {
    int UID = 2;
    initOscillator();
    init_pins();
    init_timer();
    LCD_init();
    initNetworkCommunication(UID);
    initSPI_EEPROM();
}

int main(int argc, char** argv) {
    init();

    LCD_print("Welcome", 0); // top line

    char ctr = 1;
    while(1) {
        LCD_print("Awww Yiss!!", 1); // bottom line
        delay_ms(2000);
        LCD_print("Couter: ", 1); // bottom line
        LCD_int(ctr++);
        delay_ms(2000);
    }
    
    
    return 1;
}