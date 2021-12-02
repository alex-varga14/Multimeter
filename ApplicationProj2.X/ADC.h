/* 
 * File:   ADC.h
 * Author: Alex School
 *
 * Created on November 4, 2021, 2:22 PM
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void);

void ADCinit(void);
unsigned int do_ADC(void);

#endif	/* ADC_H */

