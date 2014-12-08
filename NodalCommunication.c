#include <p33EP512GM710.h>
#include <string.h>
#include <stdlib.h>
#include "NodalCommunication.h"


#define FP 3750000
#define BAUDRATE 9600
#define BRGVAL ((FP/BAUDRATE)/16)-1
#define FIFO_SIZE 64 //Size of communication receive buffer

char *receivePointerU2;
int receiveBufferSizeU2 = 0;
int bufferLenU2 = 0;

//Used for the input receive character buffer
char receiveBufferU2[FIFO_SIZE];
char * inputStringU2;


void initNetworkCommunication(){
    init485Chip();
    initUART2();
    initNodalMessageReceiveInterrupt();
    FIFOinitU2();
}

void init485Chip(){

}

void initUART2(){

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
}

void initNodalMessageReceiveInterrupt(){
    IFS1bits.U2RXIF  = 0;   //U2RX flag set to 0
    IEC1bits.U2RXIE = 1;    //U2RX interrupt enabled
    IPC7bits.U2RXIP = 6;    //U2RX interrupt priority
}


void SendNodalFloat(float data){

    void *ptr = &data;
    int i;
    ptr+=3;

    for(i=3;i>=0;i--){
       putUART2(*(char *)ptr);
       ptr--;
    }
}


//Will send a string on the network
void SendNodalString(char * output){
    int len = strlen(output);
    int i;
    for(i = 0; i < len ; i++){
        putUART2(output[i]);
    }
}

void putUART2(char data){
    while(U2STAbits.UTXBF == 1);
    U2TXREG = data;
}

//Initializes receive string fifo buffer
void FIFOinitU2(void){
    bufferLenU2 = 0;
    receivePointerU2 = receiveBufferU2;
}

void FIFOputU2(char data){
   *receivePointerU2 = data;
    receivePointerU2++;
    bufferLenU2++;  
}


void flushFIFOBufferU2(void){

    inputStringU2 = (char *)malloc(bufferLenU2*sizeof(char));
    int i;
    for(i=0;i<bufferLenU2;i++){
        inputStringU2[i]=receiveBufferU2[i];
    }
     FIFOinitU2();    

    //For safety, first check that I have received a legitimate message
    //and for two that it is a message to me or a message for all nodes, represented
     //by the '*' char
     //If so, send the result over to the activity manager
    //if(strlen(inputStringU2) > 0 && (inputStringU2[0] == UID || inputStringU2[0] == '*')){
      //  activityManagerProcessMessage(inputStringU2);
    //}
}


void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt( void ){
    IFS1bits.U2RXIF  = 0;   //U2RX flag set to 0

     int data = U2RXREG;

     if(data=='\r'){
            flushFIFOBufferU2(); //Converts the input buffer into a string
     }else{
            FIFOputU2(data);
     }    


 } 















