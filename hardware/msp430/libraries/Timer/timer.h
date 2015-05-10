//
//  General timer function library
//  Functions to manage timed events, low power wait and timeouts
//
//
//  Author :  Nathan Crum
//  Date   :  Jan 10, 2015
//  Version:  1.00
//  File   :  timer.h
//
//  Based partially on the Texas Instruments FR5969 Launchpad demo code
//  
// 

#ifndef timer_h
#define timer_h

#include "Energia.h"

typedef enum {Running, Timeout} TimeoutState;

class timer {
public:
//
	timer();
	void begin();
	String WhoAmI();
	void startTimeout(uint32_t mils);	/* software watchdog timer (millisecond) */
	int checkTimeout();					/* check timer status */
//
private:
//
	TimeoutState timerstate;
	uint32_t start_time;
	uint32_t diff_time;
//
};

#endif
