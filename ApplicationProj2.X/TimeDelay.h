/* 
 * File:   TimeDelay.h
 * Author: Dominic Vandekerkhove, Alex Varga, Carter Fuchs 
 *
 * Created on November 18, 2021, 3:12 PM
 */

#ifndef TIMEDELAY_H
#define	TIMEDELAY_H

#ifdef	__cplusplus
extern "C" {
#endif

void Delay_ms(uint16_t time_ms);
void T1init();
void T2init();
void start_timer();
int get_current_ms();
int stop_timer();

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void);

#ifdef	__cplusplus
}
#endif

#endif	/* TIMEDELAY_H */

