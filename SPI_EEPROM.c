#include "SPI_EEPROM.h"
#include <p33EP512GM710.h>
#include <pps.h>
#include <spi.h>

//SPI EEPROM DEFINES
#define SS      LATAbits.LATA12      // slave select pin
#define SYSCLK  40000000
// Macro to get array size in bytes
// note that array size can't be found after passing pointer to a function
#define LEN(x)  (sizeof(x) / sizeof(x[0]))
// 25LC512 EEPROM instructions
#define WREN    0x06    // write enable
#define RDSR    0x05    // read STATUS register
#define WRDI    0x04    // write disable
#define WRITE   0x02    // initialize start of write sequence
#define READ    0x03    // initialize start of read sequence
#define CE      0xc7    // erase all sectors of memory



void initSPI_EEPROM_PPS(){

    unsigned int SPICONValue;
    unsigned int SPISTATValue;
    unsigned int SPIFRAMEValue;

    CloseSPI2();
    
    TRISGbits.TRISG6 = 0;
    TRISGbits.TRISG7 = 1;
    TRISGbits.TRISG8 = 0;
    TRISAbits.TRISA12 = 0;

    PPSInput(IN_FN_PPS_SDI2, IN_PIN_PPS_RPI119);
    PPSOutput(OUT_FN_PPS_SCK2, OUT_PIN_PPS_RP120);    
    PPSOutput(OUT_FN_PPS_SDO2, OUT_PIN_PPS_RP118);

   
    
    SPICONValue = ENABLE_SDO_PIN &
                        ENABLE_SCK_PIN &
                        SPI_MODE16_OFF &
                        SPI_SMP_OFF &
                        SPI_CKE_ON &
                        SLAVE_ENABLE_OFF &
                        CLK_POL_ACTIVE_HIGH &
                        MASTER_ENABLE_ON &
                        SEC_PRESCAL_7_1 &
                        PRI_PRESCAL_64_1;

    SPIFRAMEValue = FRAME_ENABLE_OFF;

    SPISTATValue = SPI_ENABLE &
                        SPI_IDLE_CON &
                        SPI_RX_OVFLOW_CLR;

    OpenSPI2(SPICONValue, SPIFRAMEValue, SPISTATValue );

    SS=1;


    //IEC2bits.SPI2IE = 0;

}







//Following are written by Dan Strawser
void initSPI_EEPROM(){

    PPSUnLock;

    ANSELGbits.ANSG6=0;
    ANSELGbits.ANSG7=0;
    ANSELGbits.ANSG8=0;
    ANSELAbits.ANSA12=0;

    TRISGbits.TRISG6 = 0;
    TRISGbits.TRISG7 = 1;
    TRISGbits.TRISG8 = 0;

    TRISAbits.TRISA12 = 0;

    RPOR11bits.RP120R = 9;  //RP120, PIN 12, as SCK2
    RPOR10bits.RP118R = 8;  //RP118, PIN 10, as SDO2
    RPINR22bits.SDI2R = 119;    //RP119, PIN 11, as SDI2

    PPSLock;

    //RPOR3bits.RP40R = 9; //RP40, PIN 74 as SCK2
    //RPOR2bits.RP39R = 8; //RP39, PIN 72, as SDO2
    //RPINR22bits.SDI2R = 61; //RPI61, PIN 73, as SDI2


    SS = 1;
    // SPI config
    // CKP (clock polarity control) = 0
    // CKE (clock edge control) = 1
    // 8-bit, Master Mode
    // Baud = 4MHz = Fpb/( 2*( SPI1BRG+1)) = 40MHz/( 2*( 4+1))
    SPI2CON1=0;
    SPI2CON2=0;
    SPI2STAT=0;

    SPI2CON1bits.DISSCK=0;
    SPI2CON1bits.DISSDO = 0;
    SPI2CON1bits.MODE16=0;
    SPI2CON1bits.SMP=0;
    SPI2CON1bits.CKE=1;
    SPI2CON1bits.CKP=0;   
    SPI2CON1bits.SSEN = 0;
    SPI2CON1bits.SPRE = 0;
    SPI2CON1bits.PPRE=0;
    SPI2CON1bits.MSTEN=1;
    
    SPI2STATbits.SPIEN=1; 
    

}

// This is a function that will initialize the UID and other variables
// unique to the device.  It shoud only be run when rewriting the entire
// network and not on every upload
void initializeDevice(){
    char writeUID[] = "Z";
    eraseEEPROM();
    writeEEPROM( 180, writeUID, sizeof(writeUID) / sizeof(writeUID[0]) );
}

//Returns the UID of this device stored in EEPROM
char getUID(){
    char readUID[1];
    readEEPROM( 180, readUID, sizeof(readUID) / sizeof(readUID) );
    return readUID[0];
}

// Following are mostly from an SPI library

// SPI2_transfer() - write to and read from SPI2 buffer
char SPI2_transfer( char b){
   // SPI2BUF = b;                        // write to buffer for TX
    SPI2BUF = b;                        // write to buffer for TX
    while( !SPI2STATbits.SPIRBF);       // wait transfer complete
    return SPI2BUF;                     // read the received value
} // END SPI2_transfer()


// waitBusy() - checks if EEPROM is ready to be modified and waits if not ready
int waitBusy()
{
    char status = 0;

    do{
        SS = 0;                         // Select EEPROM
        SPI2_transfer( RDSR);           // Read EEPROM status register       
        status = SPI2_transfer( 0);     // send dummy byte to receive incoming data
        SS = 1;                         // Release EEPROM
    }while( status & 0x01);             // write-in-progress while status<0> set to '1'

    return 0;
} // END waitBusy()


/* readEEPROM() - reads data from EEPROM
 *
 * Inputs:  address - address from 0-0xffff (64kB / 512kbits)
 *          loadArray - array to load EEPROM data to
 *          loadArray_size - number of bytes of EEPROM data to load into array */
int readEEPROM( unsigned short address, char* loadArray, unsigned short loadArray_size)
{
    unsigned short i;

    // Wait until EEPROM is not busy
    waitBusy();

    SS = 0;                         // Select EEPROM
    SPI2_transfer( READ);           // initiate read
    SPI2_transfer( address >> 8);   // address must be 16-bits but we're transferring it in two 8-bit sessions
    SPI2_transfer( address);

    // Request and store loadArray_size number of bytes into loadArray
    for( i=0 ; i<loadArray_size ; i++){
        loadArray[i] = SPI2_transfer( 0x00);    // send dummy byte to read 1 byte
    }
    SS = 1;                         // Release EEPROM

    return 0;
} // END readEEPROM()

/* writeEEPROM() - write data to EEPROM
 *
 * Inputs:  address - address from 0-0xffff (64kB / 512kbits)
 *          storeArray - array of which contents are stored in EEPROM
 *          storeArray_size - number of bytes in array to store into EEPROM */
int writeEEPROM( unsigned short address, char* storeArray, unsigned short storeArray_size)
{
    unsigned short i;

    // Wait until EEPROM is not busy
    waitBusy();

    SS = 0;                 // Select EEPROM
    SPI2_transfer( WREN);   // Send WRITE_ENABLE command
    SS = 1;                 // Release EEPROM
    SS = 0;                 // Select EEPROM again after WREN cmd
    SPI2_transfer( WRITE);  // initiate write
    SPI2_transfer( address >> 8 );
    SPI2_transfer( address );
    // write 1 byte at a time from array
    // Big endian - MSB at lowest address (0 - first letter in string)
    for( i=0 ; i<storeArray_size; i++){
        // New write cycle must be initiated if writing across 128-byte pages
        if( (address+i)%128 == 0 && i!=0 ){
            SS = 1;                 // Release EEPROM
            waitBusy();
            SS = 0;                 // Select EEPROM
            SPI2_transfer( WREN);   // Send WRITE_ENABLE command
            SS = 1;                 // Release EEPROM
            SS = 0;                 // Select EEPROM again after WREN cmd
            SPI2_transfer( WRITE);  // initiate write
            SPI2_transfer( (address+i) >> 8 );
            SPI2_transfer( address+i );
        }
        SPI2_transfer( storeArray[i]);
    }
    SS = 1;                         // Release EEPROM

    return 0;
} // END writeEEPROM()

/* eraseEEPROM() - erase entire contents of EEPROM
 *
 * See 25LC512 datasheet for ways to protect sections of data */
int eraseEEPROM()
{
    // Wait until EEPROM is not busy
    waitBusy();

    SS = 0;                 // Select EEPROM
    SPI2_transfer( WREN);   // Send WRITE_ENABLE command
    SS = 1;                 // Release EEPROM
    SS = 0;                 // Select EEPROM again after WREN cmd
    SPI2_transfer( CE);     // send CHIP_ERASE command
    SS = 1;                 // Release EEPROM

    return 0;
} // END eraseEEPROM()
