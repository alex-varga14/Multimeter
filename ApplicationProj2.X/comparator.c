#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <p24F16KA101.h>
#include "comparator.h"
#include "UART2.h"

//Initializse all comapartor pins
void comparatorInit(){
    //set up reference oscillator
    REFOCONbits.RODIV = 0b1111;
    REFOCONbits.ROEN = 1;
    TRISBbits.TRISB15 = 0;
    
    //set up comparator input
    TRISBbits.TRISB1 = 1;
    AD1PCFGbits.PCFG3 = 0;
    
    IEC1bits.CMIE = 0; 		// IE Off so no interrupt occurs from setup
    CM1CONbits.COE = 0; 	// Disable output pin
    CM1CONbits.CPOL = 0; 	// Standard sense. +In High ==> Out High
    CM1CONbits.EVPOL = 2; 	// Event detected on output edge falling
    CM1CONbits.CREF = 1; 	// +IN is internal CVRef
    CM1CONbits.CCH = 1; 	// -IN is C1INB Pin
    CM1CONbits.CON = 1; 	// Turn Comparator ON
    
    IPC4bits.CMIP = 7; // set comparator interrupt priority (lowest))
    
    CVRCON = 0; 		// CVRef = 0
    
    CM1CONbits.CEVT = 0;
    IFS1bits.CMIF = 0; 		// Clear IF after set-up
    IEC1bits.CMIE = 1;      //enable comparator interrupts
}

//ISR for Comparator
void __attribute__((interrupt, no_auto_psv)) _CompInterrupt(void)
{
	IFS1bits.CMIF = 0;		// clear IF flag
        
    return;
}