/* 
 * File:   IOs.h
 * Author: carte
 *
 * Created on November 30, 2021, 11:43 AM
 */

#ifndef IOS_H
#define	IOS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    void IOinit();
    void IOcheck();
    void __attribute__ ((interrupt, no_auto_psv)) _CNInterrupt (void);

#ifdef	__cplusplus
}
#endif

#endif	/* IOS_H */

