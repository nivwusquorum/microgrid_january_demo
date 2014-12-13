/* 
 * File:   send_to_arduino.h
 * Author: sidor
 *
 * Created on December 12, 2014, 7:35 PM
 *
 * It's designed to work with the following arduino code:
 *
 *  ============================================================================
int command_pin = 4;     // potentiometer wiper (middle terminal) connected to analog pin 3
int data_pin = 3;
int reset_pin = 5;                       // outside leads to ground and +5V

unsigned long val = 0;           // variable to store the value read
unsigned long temp1, temp2, temp3;

unsigned long result_number = 0;
unsigned long next_bit = 0;
unsigned long result = 0;
unsigned long last_result = 0;
unsigned long state = 0;

const int ALPHA_NUM = 1;
const int ALPHA_DENOM = 10;

const int LOW_THRESHOLD = 100;
const int HIGH_THRESHOLD = 450;
const int MID_THRESHOLD = (LOW_THRESHOLD + HIGH_THRESHOLD)/2;

const int WORD_SIZE = 32;

const long BATCH_WAIT_MS = 2000;
int break_printed = false;
long last_time_ms = 0;

void setup()

{

  Serial.begin(9600);          //  setup serial
  int cur_time = millis();
  while (!Serial) ;
  delay(100);
  Serial.print("Serial initialized in ");
  Serial.print(millis()-cur_time);
  Serial.println(" milliseconds.");
  delay(1000);
  Serial.println("Ready to capture.");
}

void read_data() {
   if (next_bit < WORD_SIZE) {
     temp1 = analogRead(data_pin);

     if (temp1 <= MID_THRESHOLD) {
       temp2 = 0;
     } else if (temp1 > MID_THRESHOLD) {
       temp2 = 1;
     }
     result |= (temp2 << next_bit);
     next_bit ++;
     if (next_bit == WORD_SIZE) {
       print_result(result);
       last_result = result;
     }
   }
}

void print_result(unsigned long no) {
  last_time_ms = millis();
  break_printed = false;

  Serial.print("Result number ");
  Serial.print(result_number);
  result_number++;
  Serial.print(' ');
  for (long i=WORD_SIZE; i>=0; --i) {
   Serial.print((no & ((long)1<<i)) >> i);
  }
  Serial.print (' ');
  for(long i=7; i>=0; --i) {
    Serial.print((no & ((long)0xF<<(4*i))) >> ((long)4*i), HEX);
  }
  Serial.print(' ');
  Serial.println(result);
}

void loop() {
  if (millis() - last_time_ms > BATCH_WAIT_MS && !break_printed) {
      Serial.println(" ");
      Serial.println("NEW BATCH");
      break_printed=true;
  }
  if (analogRead(reset_pin) > HIGH_THRESHOLD) {
    next_bit = 0;
    result = 0;
    last_result = 0;
    state = 0;
    val = 0;
  } else {
    val = analogRead(command_pin);    // read the input pin
    val = ((ALPHA_DENOM-ALPHA_NUM)*val + ALPHA_NUM*val)/ALPHA_DENOM;
    if (state == 0 && val > HIGH_THRESHOLD) {
      state = 1;
      read_data();
    } else if (state == 1 && val < LOW_THRESHOLD) {
      state = 0;
      read_data();
    }
  }
}
================================================================================
 */

#ifndef SEND_TO_ARDUINO_H
#define	SEND_TO_ARDUINO_H

#include <p33EP512GM710.h>


// MAKE SURE ALL TREE ARE INITIALIZED AS OUTPUT AND DIGITAL.
#define RESET LATBbits.LATB13
#define COMMD LATAbits.LATA7
#define DATA LATBbits.LATB15



void send_to_arduino(unsigned long x) {
    int del;
    COMMD = 0;
    RESET = 1;
    del = 5000;
    while(del--);
    RESET = 0;
    int count = 32;

    while (count > 0) {
        DATA = x & 0x01;
        COMMD = !COMMD;
        del = 5000;
        while(del--);
        count-=1;
        x = x >> 1;
    }

    del = 5000;
    while(del--);
}



#endif	/* SEND_TO_ARDUINO_H */

