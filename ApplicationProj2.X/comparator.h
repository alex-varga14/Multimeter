/* 
 * File:   comparator.h
 * Author: carte
 *
 * Created on December 7, 2021, 2:56 PM
 */

#ifndef COMPARATOR_H
#define	COMPARATOR_H

#ifdef	__cplusplus
extern "C" {
#endif

    void comparatorInit();
    void __attribute__((interrupt, no_auto_psv)) _CompInterrupt(void);

#ifdef	__cplusplus
}
#endif

#endif	/* COMPARATOR_H */

