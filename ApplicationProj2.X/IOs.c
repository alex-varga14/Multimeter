/* 
 * File:   IOs.c
 * Author: Dominic Vandekerkhove, Alex Varga, Carter Fuchs 
 *
 * Created on October 7, 2021, 1:54 PM
 */

#include <xc.h>
#include <p24F16KA101.h>
#include "IOs.h"
#include "UART2.h"
#include "multimeter.h"

//Button flags:
int CN30flag = 0;// flag for PB1
int CN1flag = 0;// flag for PB2
int CN0flag = 0;// flag for PB3

void IOinit() {
    AD1PCFG = 0xFFFF;
    
    TRISAbits.TRISA2 = 1;   //set PB1 as input
    CNPU2bits.CN30PUE = 1;  //pull up PB1
    CNEN2bits.CN30IE = 1;   //enable CN30 interrupt
    
    TRISBbits.TRISB4 = 1;   //set PB2 as input
    CNPU1bits.CN0PUE = 1;   //pull up PB2
    CNEN1bits.CN1IE = 1;    //enable CN1 interrupt
    
    TRISAbits.TRISA4 = 1;   //set PB3 as input
    CNPU1bits.CN1PUE = 1;   //pull up PB3
    CNEN1bits.CN0IE = 1;    //enable CN0 interrupt
    
    TRISBbits.TRISB8 = 0;   //set LED pin as output
    
    IPC4bits.CNIP = 4;      //set CN interrupt priority
    IFS1bits.CNIF = 0;      //clears the CN interrupt flag
    IEC1bits.CNIE = 1;      //enable the CN interrupt (general)
}

void IOcheck() {
    /* states:
     * 0 - Measure and display frequency
     * 1 - Measure and display voltage
     * 2 - Measure and display resistance
     * 3 - Measure and display capacitance
     */
    if(CN30flag) {
        float voltage = measure_voltage();   
        Disp2String("VOLTMETER Voltage =");
        Disp2Float(voltage);
        Disp2String("V");
    } else if(CN1flag) {
        //PB2 is pressed
        float resistance = measure_resistance();
        Disp2String("OHMMETER Resistance =");
        Disp2Dec(resistance);
        char* outp;
        sprintf(outp, "%s", "\u03a9");
        Disp2String(outp);
    } else if (CN0flag) {
        float capacitance = measure_capacitance();
        Disp2String("Capacitance =");
        Disp2Dec(capacitance);
        Disp2String("uF");
    } else {
        float frequency = measure_frequency();
        Disp2String("Frequency =");
        Disp2Dec(frequency);
        Disp2String("Hz");
    }
}

void __attribute__ ((interrupt, no_auto_psv)) _CNInterrupt (void)
{
    if(IFS1bits.CNIF == 1)
    {   
        if(PORTAbits.RA2==0) {    // PB1 pressed
            CN30flag = 1; 
        }
        else {                    // PB1 not pressed
            CN30flag = 0;
        }
        if (PORTBbits.RB4 == 0) // PB2 pressed
            CN1flag = 1; 
        else                    // PB2 not pressed
            CN1flag = 0;
        if (PORTAbits.RA4 == 0) { // PB3 pressed
            CN0flag = 1;
        }
        else {                  // PB3 not pressed
            CN0flag = 0;
        }
    }
    
    IFS1bits.CNIF = 0; // clear IF flag
    Nop();
    
    return;
}
