/* 
 * File:   NodalCommunication.h
 * Author: Dan Strawser
 *
 * This is a package which controls communication between uLinks
 *
 * Created on October 17, 2014, 4:35 PM
 */

#ifndef NODALCOMMUNICATION_H
#define	NODALCOMMUNICATION_H

extern int testGlobal;

void initNetworkCommunication();
void SendNodalString(char * msg);
void SendNodalFloat(float data);


void init485Chip();
void initUART2();
void initNodalMessageReceiveInterrupt();
void putUART2(char data);
void FIFOputU2(char data);
void FIFOinitU2();
void flushFIFOBufferU2();

//void initUART2(char sender);
//void init485();
//void initMessageReceiveInterrupt();
//void putCharU2(char c);
//void putIntU2(int message);
//void sendMessage(char * output);

//void FIFOinitU2(void);
//void FIFOputU2(char input);
//void flushFIFOBufferU2(void);
//void __ISR(_UART2_VECTOR, ipl2) IntUart2Handler(void);

#endif	/* NODALCOMMUNICATION_H */

