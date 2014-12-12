/*
 * File:   main.c
 * Author: Dan
 *
 * Created on November 24, 2014, 12:59 PM
 * Code for dsPIC33E, comm board
 */

#include <p33EP512GM710.h>
#include "SPI_POWER_BOARD.h"
#include "SPI_EEPROM.h"
#include "NodalCommunication.h"


// Internal FRC Oscillator
_FOSCSEL(FNOSC_FRCPLL);  // FRC Oscillator

_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF  & POSCMD_NONE);   // Clock Switching is enabled and Fail Safe Clock Monitor is disabled
						     // OSC2 Pin Function: OSC2 is Clock Output
                                                     // Primary Oscillator Mode: Disabled
_FWDT(FWDTEN_OFF);              // Watchdog Timer Enabled/disabled by user software
_FGS(GWRP_OFF); // General segment Write protect turned off
_FICD(ICS_PGD2 & JTAGEN_OFF) // Select Debug/Program Lines and JTAG OFF
        

int LED_FLAG = 0;
int timerCounter = 0;


void initOscillator(){
  // setup internal clock for 70MHz/35MIPS
  // 7.37/2=3.685*43=158.455/2=79.2275
  CLKDIVbits.PLLPRE=0;        // PLLPRE (N2) 0=/2
  //PLLFBD=41;                  // pll multiplier (M) = +2
  //PLLFBD = 41;  //PUT BACK TO 41!

  PLLFBD = 35;  //PUT BACK TO 41
  CLKDIVbits.PLLPOST=0;       // PLLPOST (N1) 0=/2

  RCONbits.SWDTEN = 0;


// __builtin_write_OSCCONH(0x01);
// __builtin_write_OSCCONL(0x01);

   //while (OSCCONbits.COSC != 0b011);

 // while(!OSCCONbits.LOCK);    // wait for PLL ready
  //LATAbits.LATA0 = 1;

  //Init timers
  //T1CONbits.TON = 0;
  //T1CONbits.TCKPS = 11;
  //T1CONbits.TCS = 0;
  //PR1 = 65000;
  //TMR1 = 0x00;

  //IFS0bits.T1IF = 0;
  //IPC0bits.T1IP = 1;
  //IEC0bits.T1IE = 1;

  //T1CONbits.TON = 1;
}

void delay(float cycles){
    int i;int j;int k;
    int p=0;
    for(i = 0;i<cycles;i++){
        for(j=0;j<cycles;j++){
            for(k=0;k<100;k++){
           }
        }
    }
}

#define WREN2    0x06    // write enable
#define RDSR2    0x05    // read STATUS register
#define SS2_2      LATCbits.LATC10

int main(int argc, char** argv) {

    int UID = 2; //Should be on EEPROM but that probably doesn't work
    initOscillator();
    int w1;    
    //initNetworkCommunication(UID);
    initSPI_EEPROM();
    int w6;
    //initSPI_PowerBoard();

    unsigned int msg9 = 'a';
    unsigned int msg1 = 5;




    while(1){

        //sendIntPowerBoard(msg9);

        /*
        if(UID==1){
           msg1 = pollPowerBoard();
           putUART2(msg1);
        }

        if(UID==2){
            //U2STAbits.UTXEN = 1; // Enable UART TX
            LATCbits.LATC7 = 1;
            putUART2('n');
            LATCbits.LATC7 = 0;
            //U2STAbits.UTXEN = 0; // Enable UART TX

        } 

         */
        SS2_2 = 0;
        SPI2_transfer(WREN2);
        SS2_2 = 1;
        delay(10);
        SS2_2 = 0;
        SPI2_transfer(RDSR2);
        int ii;
        int i = SPI2_transfer(0);
        SS2_2 = 1;
        int i6=i;

        delay(200);
    }
    
    return 1;
}