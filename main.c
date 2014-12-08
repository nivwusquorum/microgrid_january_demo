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
_FOSCSEL(FNOSC_FRC);  // FRC Oscillator

//_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF  & POSCMD_NONE);   // Clock Switching is enabled and Fail Safe Clock Monitor is disabled
						     // OSC2 Pin Function: OSC2 is Clock Output
                                                     // Primary Oscillator Mode: Disabled
_FWDT(FWDTEN_OFF);              // Watchdog Timer Enabled/disabled by user software
_FGS(GWRP_OFF); // General segment Write protect turned off
_FICD(ICS_PGD2 & JTAGEN_OFF) // Select Debug/Program Lines and JTAG OFF
        

int LED_FLAG = 0;
int timerCounter = 0;

/*
void __attribute__((interrupt, auto_psv)) _T1Interrupt( void )
{

    if(timerCounter == 10){

        if(LED_FLAG){
            LATAbits.LATA0 = 0;
            LED_FLAG = 0;
        }else{
            LATAbits.LATA0 = 1;
            LED_FLAG = 1;
        }

        timerCounter = 0;

    }

    timerCounter++;



    IFS0bits.T1IF = 0;
} */


void initOscillator(){
  // setup internal clock for 80MHz/40MIPS
  // 7.37/2=3.685*43=158.455/2=79.2275
  CLKDIVbits.PLLPRE=0;        // PLLPRE (N2) 0=/2
  //PLLFBD=41;                  // pll multiplier (M) = +2
  //PLLFBD = 41;  //PUT BACK TO 41!

  PLLFBD = 41;  //PUT BACK TO 41
  CLKDIVbits.PLLPOST=0;       // PLLPOST (N1) 0=/2




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

/*
void initUART2Computer(){
    RPINR18bits.U1RXR = 24;  //RPI 24 as U1RX, PIN 49
    RPOR1bits.RP36R = 1; //RP36 as U1TX, PIN 50

    U1MODEbits.STSEL = 0; // 1-Stop bit
    U1MODEbits.PDSEL = 0; // No Parity, 8-Data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud disabled
    U1MODEbits.BRGH = 0; // Standard-Speed mode

    U1BRG = BRGVAL;

    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UART TX
} 

void SendMessageU1(char msg){
    while(U1STAbits.UTXBF == 1);
    U1TXREG = msg;
}

void initRS485(){

    TRISCbits.TRISC7 = 0;   //DRIVE ENABLE AS OUTPUT, C7, PIN78
    LATCbits.LATC7 = 1;     //DRIVE ENABLE HIGH, C7, PIN78

    TRISBbits.TRISB9 = 1;   //PIN 76, RB9 as INPUT
    TRISCbits.TRISC6 = 0;   //PIN 77, RC6 as OUTPUT

    RPINR19bits.U2RXR = 41;  //RPI 41, PIN 76, as U2RX
    RPOR6bits.RP54R = 3; //RP54, PIN 77, aS U2TX

    U2MODEbits.STSEL = 0; // 1-Stop bit
    U2MODEbits.PDSEL = 0; // No Parity, 8-Data bits
    U2MODEbits.ABAUD = 0; // Auto-Baud disabled
    U2MODEbits.BRGH = 0; // Standard-Speed mode

    U2BRG = BRGVAL;

    U2MODEbits.UARTEN = 1; // Enable UART
    U2STAbits.UTXEN = 1; // Enable UART TX

} */

/*
void SendMessageNetwork(char msg){
    while(U2STAbits.UTXBF == 1);
    U2TXREG = msg;
 }*/

void delay(int cycles){
    int i;int j;int k;
    int p=0;
    for(i = 0;i<cycles;i++){
        for(j=0;j<cycles;j++){
            for(k=0;k<100;k++){
           }
        }
    }
} 



 
int main(int argc, char** argv) {
  
    initOscillator();   
    initNetworkCommunication();

    int i;

    while(1){      

        for(i=0;i<3;i++){
           putUART2(17);
        }
        
        SendNodalFloat(7.5);
        delay(100); //Random delay
        for(i=0;i<3;i++){
           putUART2(17);
        }
        SendNodalFloat(2.5);
        delay(100);

    }


    
    return 1;
}