#include <xc.h>
#include <p24F16KA101.h>

void T1init() {
    //T1 config
    T1CONbits.TSIDL = 0; // operate in idle mode
    T1CONbits.TCS = 0; // use internal clock
    
    //T1CON prescaler
    T1CONbits.TCKPS0 = 1; 
    T1CONbits.TCKPS1 = 1;
    
    // Timer 1 interrupt config
    IPC0bits.T1IP = 7; // set priority for interrupt
    IEC0bits.T1IE = 1; //enables timer interrupt
    IFS0bits.T1IF = 0; // clears timer 2's flag
}

void T2init() {
    //T2 config
    T2CONbits.TSIDL = 0; // operate in idle mode
    T2CONbits.T32 = 1; // operate timer2 as 32-bit timer
    T2CONbits.TCS = 0; // use internal clock
    
    //T2CON prescaler
    T2CONbits.TCKPS0 = 1; 
    T2CONbits.TCKPS1 = 1;
}


//Note: Maximum time_ms = 4194
void Delay_ms(uint16_t time_ms) {
    T1init();
    //time_ms is in milliseconds, need to convert to PR2
    uint16_t PR1val = (int)time_ms*15.625;
    /*
     1/(prescaler*(2/fosc))
     = 1/(256*(0.25 us))
     ~= 16
     units are 1/ms because this value is multiplied by time_ms which is in milliseconds
     */
    
    //CLKDIV = 0; // change default timing from 2:1 to 1:1
    
    PR1 = PR1val; //PR2 = time_ms translated to number of cycles
    TMR1 = 0; //start with timer 1 = 0
    
    T1CONbits.TON = 1; //start timer 1
    
    Idle(); //wait until interrupt
    
    T1CONbits.TON = 0; //stop timer 1
    TMR1 = 0;
    
    return;
}

//starts timer 2/3
void start_timer() {
    T2init();
    TMR2 = 0; //start counting from 0
    TMR3 = 0;
    T2CONbits.TON = 1; //start timer 2/3
}

//returns TMR2 value in ms but doesn't stop the timer
int get_current_ms() {
    int32_t total_cycles = TMR3*65536 + TMR2;
    int millis = total_cycles/15.625;
    return millis;
}

//stops timer 2/3 and returns the value of TMR2 in milliseconds
int stop_timer() {
    T2CONbits.TON = 0; //stop timer 2
    //convert from number of clock cycles to milliseconds: conversion value
    //is 15.625 (see comment in Delay_ms())
    int32_t total_cycles = TMR3*65536 + TMR2;
    int millis = total_cycles/15.625;
    return millis;
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0; //clear T2 interrupt flag
    return;
}
