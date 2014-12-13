#include "timer.h"


void init_timer(void) {
    T2CONbits.TON = 0;       // Disable Timer
    T2CONbits.TCS = 0;       // Select internal instruction cycle clock
    T2CONbits.TGATE = 0;     // Disable Gated Timer mode
    T2CONbits.TCKPS = 0b000; // Select 1:1 Prescaler
    T2CONbits.T32 = 1;       // Enable 32 bit mode
    T2CONbits.TSIDL = 0;     // tick when idle
    T3CONbits.TSIDL = 0;     // ditto
    TMR3HLD = 0x00;          // reset timer register
    TMR2 = 0x00;             // ditto
    //IPC0bits.T1IP = 0x01; // Set Timer 1 Interrupt Priority Level
    //IFS0bits.T1IF = 0; // Clear Timer 1 Interrupt Flag
    ///IEC0bits.T1IE = 1; // Enable Timer1 interrupt
    T2CONbits.TON = 1; // Start Timer
}

// Inline to make next cycle be time zero.
inline void reset_time() {
    TMR3HLD = 0x00;
    TMR2 =    0x00;
}

inline long timer_cycles(void) {
     long lsw = TMR2;
     long msw = TMR3HLD;
     return msw*((long)1<<(long)16) + lsw;
}

void delay_cycles(long cycles) {
    reset_time();
    // ideally one would subtract execution of get_time()...
    while(timer_cycles()<cycles);
}


void delay_ms(long ms) {
    delay_cycles(ms * (CYCLES_PER_SECOND/1000L));
}

void delay_us(long us) {
    delay_cycles(us * (CYCLES_PER_SECOND/1000000L));
}

void delay_ns(long ns) {
    // This funny formula tries to avoid overflow for large number of ns.
    delay_cycles((ns * (CYCLES_PER_SECOND/1000000L))/1000L );

}
