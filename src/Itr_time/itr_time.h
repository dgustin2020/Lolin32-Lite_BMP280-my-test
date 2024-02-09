/*
itr_time.h
*/
#ifndef ITR_TIME_H
#define ITR_TIME_H
#include <header.h>

/****/
#define PRESCALER_TIMER_1000    16//4//prescaler per avere clock a 5 MHz --> TIK = 0.2 uSec
#define TIK_TIMER2_X_1SEC       (CLOCK_CPU / PRESCALER_TIMER_1000) // 5000000 -->  0.2uSec
#define MICROSEC_X_ITR          1000 // 1000uSec --> 1 mSec
#define TIC_TIMER_1MSEC         ((CLOCK_CPU / PRESCALER_TIMER_1000) / MICROSEC_X_ITR) // n_clock per avere itr a 1mSec 
                                //5000 // ((80MHz / 16 ) / 5000) = 1000Hz --> 1mSec
#define TIC_TIMER_1MSEC_SEL     500                                
/****/
#define L_MILL                  1


/* procedure */
extern void     itr_time_init(FnPtr gest_itr, uint8_t timer_usato, uint16_t divisore_usato, uint64_t n_cicli);
extern uint64_t itr_time_read_counter(void);
extern bool     itr_time_scaduto(void);
extern void     itr_time_scaduto_clear(void);

#endif