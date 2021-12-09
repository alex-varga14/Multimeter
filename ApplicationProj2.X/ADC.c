/* 
 * File:   ADC.c
 * Author: Alex School
 *
 * Created on November 4, 2021, 2:20 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include <xc.h>
#include "ADC.h"
#include "UART2.h"
#include "math.h"

//uint16_t ADCValues[1000];

void ADCinit(void)
{
    AD1CON1bits.ADON =0; //Turn off ADC, turn on before sampling in do_ADC()
    AD1CON1bits.ADSIDL =0; //Continue adc operation in idle mode
    AD1CON1bits.FORM = 0b00; //0b00: Int, 01: Signed Int, 10: Fractional, 11: Signed fract
    AD1CON1bits.SSRC = 0b111; //Start ADC conversion time set my SAMC has passed
    AD1CON1bits.ASAM = 0; //Next sampling begins when SAMP bit is set
    AD1CON1bits.SAMP = 0; //set this bit in ADC routine to start sampling
    
    //Sampling time and ADC clock select
    AD1CON2bits.VCFG = 0b000; //Vref += AVDD   Vref -=AVSS
    AD1CON2bits.CSCNA = 0; //Do not scan inputs; use channel selected by CHOSA bits
    AD1CON2bits.SMPI = 0b0000; //Any ADC interrupts occur at completion of each sample/covert seq
    AD1CON2bits.BUFM = 0; //Buffer configured as 1 16 word buffer
    AD1CON2bits.ALTS = 0; //Always use MUXA input MUX settings
    AD1CON3bits.ADRC = 0; //0: Use system clk 1: ADC uses internal RC clock for idle or sleep mode
    AD1CON3bits.SAMC = 0b01111; //Sample time = 15*TAD = 31*1/125e3 = 0.124ms
    AD1CON3bits.ADCS = 0b00111111; //ADCS conversion clock select bits 64 Tcy. Ignored if using internal AD RC clock
    
    //TAD = (2/8MHz) * 64 = 0.016ms
    //SAMC = 31*0.016 ms = 0.496ms
    //1 conversion in 12 TADs : conv = 0.192 ms
    //full conversion = -0.6 ms
    //250 Hz = 4ms period
    
    
    AD1CSSL = 0; //Input scan disabled, 0x0000 is default state
    
    //INT bit setup
    IPC3bits.AD1IP = 7;
    
    AD1CON1bits.ADON = 1; //Turn on ADC module
    
    return;
}


//ADC conversion subroutine
//Returns single conversion 10bit ADC value (ADCvalue) in unsigned int form

unsigned int do_ADC(int pin)
{
    AD1CHSbits.CH0NB = 0; //CHO MUXB negative input is VR-
    AD1CHSbits.CH0SB = pin; //Positive input is AN5/RA3/pin8 for MUXB
    AD1CHSbits.CH0NA = 0; //CHO MUXA negative input is VR-
    AD1CHSbits.CH0SA = pin; //Positive input is AN5/RA3/pin8 for MUXA
    
    //IO PORT SELECTION
    if(pin == 5) {
        TRISAbits.TRISA3 = 1; //set pin8/RA3/AN5 as input
        AD1PCFG = 0xFFFF; //set all bits as digital
        AD1PCFGbits.PCFG5 = 0; //set only pin8/RA3/AN5 as analog input for ADC
    } else if (pin == 11) {
        TRISBbits.TRISB13 = 1;
        AD1PCFG = 0xFFFF;
        AD1PCFGbits.PCFG5 = 0; 
    } else if (pin == 12) {
        TRISBbits.TRISB12 = 1; // set AN12/RB12 as input
        AD1PCFG = 0xFFFF; //set all bits as digital
        AD1PCFGbits.PCFG12 = 0;
    }
    
    IFS0bits.AD1IF = 0;
    unsigned int ADCvalue = 0;
    AD1CON1bits.SAMP = 1; //start sampling, conversion starts automatically after SSRC and SAMC settings
    while(AD1CON1bits.DONE == 0) {}
    
    AD1CON1bits.SAMP = 0; //stop sampling
    
    ADCvalue = ADC1BUF0;
    
    return(ADCvalue);
}


//ADC Interrupt subroutine

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void)
{
    IFS0bits.AD1IF = 0; //clear the ADC1 interrupt flag
}

