/*
 * File:   main.c
 * Author: Dan
 *
 * Created on November 24, 2014, 12:59 PM
 * Code for dsPIC33E, comm board
 */

#include <p33EP512GM710.h>

#include "NodalCommunication.h"
#include "send_to_arduino.h"
#include "SPI_POWER_BOARD.h"
#include "SPI_EEPROM.h"
#include "timer.h"


/*
 * short 2
 * int 2
 * long 4
 * long long 8
 */

#define TRIS_INPUT 1
#define TRIS_OUTPUT 0

#define LED LATBbits.LATB14
#define TRIS_LED TRISBbits.TRISB14

#define AUX LATAbits.LATA0
#define AUX2 LATAbits.LATA11
#define AUX3 LATAbits.LATA12
#define TRIS_AUX TRISAbits.TRISA0
#define TRIS_AUX2 TRISAbits.TRISA11
#define TRIS_AUX3 TRISAbits.TRISA12


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
    TRIS_LED = TRIS_OUTPUT;
    TRIS_AUX = TRIS_OUTPUT;
    TRIS_AUX2 = TRIS_OUTPUT;
    TRIS_AUX3 = TRIS_OUTPUT;

    // All pins digital for starts.
    ANSELA = 0; // PORTA pins digital
    ANSELB = 0; // PORTB pins digital
}



void init(void) {
    initOscillator();
    init_pins();
    init_timer();
}

int main(int argc, char** argv) {
    init();

    //int UID = 2;
    //initNetworkCommunication(UID);
    //initSPI_EEPROM();
    
    send_to_arduino(0xF1F2F3F4L);

    while(1) {
        LED = 1;
        delay_ms(500);
        LED = 0;
        delay_ms(500);
    }
    
    
    return 1;
}