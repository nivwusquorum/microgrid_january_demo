/* 
 * File:   SPI_POWER_BOARD.h
 * Author: Dan
 *
 * Created on December 5, 2014, 3:09 PM
 */

#ifndef SPI_POWER_BOARD_H
#define	SPI_POWER_BOARD_H

void initSPIPowerBoard();
void sendFloatPowerBoard(float data);
void sendIntPowerBoard(int i);
void sendMessagePowerBoard(int i);

//void writeByteToPowerBoard(char a);
void delay2(int cycles);
int writeSPI1(int i);
void __attribute__((__interrupt__,auto_psv)) _SPI1Interrupt(void);


#endif	/* SPI_POWER_BOARD_H */

