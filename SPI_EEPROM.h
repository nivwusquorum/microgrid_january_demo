/*
 * File:   SPI_INTERFACE.h
 * Author: Not Dan
 *
 * Created on October 5, 2014, 10:59 PM
 */

extern char UID;


#ifndef SPI_INTERFACE_H
#define	SPI_INTERFACE_H

//Written by Dan Strawser
void initSPI_EEPROM_PPS();
void initSPI_EEPROM();
void initializeDevice();
char getUID();
char SPI2_byteToBuffer(char b);

//From library
char SPI2_transfer( char b);
int waitBusy();
int readEEPROM( unsigned short address, char* loadArray, unsigned short loadArray_size);
int writeEEPROM( unsigned short address, char* storeArray, unsigned short storeArray_size);
int eraseEEPROM();

#endif	/* SPI_INTERFACE_H */

