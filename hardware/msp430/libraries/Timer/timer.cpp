//
//  General timer function library
//  Functions to manage timed events, low power wait and timeouts
//
//
//  Author :  Nathan Crum
//  Date   :  Jan 10, 2015
//  Version:  1.00
//  File   :  timer.cpp
//
//  Original code
//  
// 

Timer_A_initUpModeParam timerA1_up_params = {
		.clockSource = TIMER_A_CLOCKSOURCE_ACLK, 					// 32.768 kHz
		.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_32,		// 1.024 kHz
		.timerPeriod = 1,											// 0.977 * (ms) ms
		.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE,
		.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
		.timerClear = TIMER_A_DO_CLEAR,
};

#include <msp430.h>
#include "timer.h"

timer::timer() {
	this->start_time = 0;
	this->diff_time = 0;
	this->timerstate = Timeout;
}

String timer::WhoAmI() {
	return "Timer object";
}

/* software watchdog timer (millisecond) */
void timer::startTimeout(uint32_t mils) {
	// Grab the current 32-bit micros value
	this->start_time = millis();

	// Reset timeout status
	this->timerstate = Running;
	this->diff_time = mils;
}

/* check watchdog timer status */
int timer::checkTimeout() {
	uint32_t cur_time;

	if(this->timerstate == Timeout)
		return 1;
		
	cur_time = millis();

	if (( cur_time - this->start_time ) > this->diff_time ) {
		this->timerstate = Timeout;
		return 1;
	}
	else
		return 0;

}

/* Low power timer delay								*
 * Exits LPM3 when user-defined ms timer expires		*/
void timer::lp_delay(uint16_t ms) {
	if (ms == 0)
		return;

	// Reset/Clear the timer clock divider, count direction, count.
	TIMER_A_clear(TIMER_A1_BASE);

	// Configure TimerA1 in up mode
	timerA1_up_params.timerPeriod = ms;								// 0.977 * (ms) ms
	Timer_A_initUpMode(TIMER_A1_BASE, &timerA1_up_params);

	 /* Start TimerA1 counter */
    TIMER_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);

    __bis_SR_register(LPM3_bits | GIE);			// Enter LPM3 w/Interrupts enabled

    TIMER_A_stop(TIMER_A1_BASE);				// Disable Timer A3
}

/* Timer1_A0 Interrupt Service Routine				*
 * Used to create a variable ms delay between operations	*/
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR_HOOK(void)
{
	__bic_SR_register_on_exit(LPM3_bits);		// Exit LPM3
}