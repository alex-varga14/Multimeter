/* 
 * File:   Multimeter.h
 * Author: carte
 *
 * Created on November 30, 2021, 11:56 AM
 */

#ifndef MULTIMETER_H
#define	MULTIMETER_H

#ifdef	__cplusplus
extern "C" {
#endif

    float measure_frequency();
    float measure_voltage();
    float measure_resistance();
    float measure_capacitance();

#ifdef	__cplusplus
}
#endif

#endif	/* MULTIMETER_H */

