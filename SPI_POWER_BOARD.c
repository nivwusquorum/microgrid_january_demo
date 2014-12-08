#include "SPI_POWER_BOARD.h"
#include "NodalCommunication.h"
#include <p33EP512GM710.h>

#define SS_POWER        LATEbits.LATE1      // slave select pin

int debugInt;

void initSPIPowerBoard(){

   ANSELB = 0; // Make all the ANx pins as digital pins
   ANSELC = 0;
   ANSELD = 0;
   ANSELE = 0;

   ANSELCbits.ANSC3=0;
   ANSELAbits.ANSA9=0;
   ANSELAbits.ANSA4=0;

   TRISCbits.TRISC3 = 0;
   TRISAbits.TRISA9 = 1;
   TRISAbits.TRISA4 = 0;

   TRISEbits.TRISE1 = 0;
   
   SS_POWER = 1;

    SPI1CON1=0;
    SPI1STAT=0;
    SPI1STATbits.SPIROV = 0;

    SPI1CON1bits.DISSCK=0;
    SPI1CON1bits.DISSDO = 0;
    SPI1CON1bits.MODE16=0;
    SPI1CON1bits.SMP=1;
    SPI1CON1bits.CKE=1;
    SPI1CON1bits.CKP=0;
    SPI1CON1bits.SSEN = 0;
    SPI1CON1bits.PPRE=0;
    SPI1CON1bits.SPRE = 0;
    SPI1CON1bits.MSTEN=1;

    SPI1CON2=0;
    SPI1STATbits.SPIEN=1;

    IFS0bits.SPI1IF = 0;    //SPI1 interrupt flag
    IEC0bits.SPI1IE = 1;    //Enable interrupt for SPI1 (KEEP DISABLED FOR NOW)
    IPC2bits.SPI1IP = 6;    //Interrupt priority for SPI1
}


void sendFloatPowerBoard(float data){
    IEC0bits.SPI1IE = 0;    //DISABLE SPI1 INTERRUPT
    void *ptr = &data;
    int i;
    ptr+=3;

    for(i=3;i>=0;i--){
        //putUART2(*(char *)ptr);
       SS_POWER = 0;
       writeSPI1(*(char *)ptr);
       SS_POWER = 1;
       ptr--;
    }
    IEC0bits.SPI1IE = 1;    //Enable interrupt for SPI1
    
}


void sendIntPowerBoard(int i){
    IEC0bits.SPI1IE = 0;    //DISABLE SPI1 INTERRUPT
    SS_POWER = 0;
    writeSPI1(i);
    SS_POWER = 1;
    IEC0bits.SPI1IE = 1;    //ENABLE SPI1 INTERRUPT
}

void sendMessagePowerBoard(int i){

    SS_POWER = 0;
    writeSPI1(i);
    SS_POWER = 1;
}

void delay2(int cycles){
    int i;int j;int k;
    int p=0;
    for(i = 0;i<cycles;i++){
        for(j=0;j<cycles;j++){
            for(k=0;k<100;k++){
           }
        }
    }
} 

int writeSPI1( int i)
{
    SPI1BUF = i; // write to buffer for TX

    while(SPI1STATbits.SPITBF);
    while( !SPI1STATbits.SPIRBF); // wait for transfer complete    
    delay(1);
   
    int bufval = SPI1BUF;
    putUART2(bufval);

    return bufval; // read the received value
}//writeSPI1


void __attribute__((__interrupt__,auto_psv)) _SPI1Interrupt(void){

    IFS0bits.SPI1IF = 0;    //SPI1 interrupt flag
    int buffer = SPI1BUF;

    //debugInt = buffer;
}
