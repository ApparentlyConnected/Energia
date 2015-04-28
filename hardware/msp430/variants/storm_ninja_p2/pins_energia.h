/*
  ************************************************************************
  *	pins_energia.h
  *
  *	Pin definition functions for Storm Ninja w/ MSP430FR5969
  *		Copyright (c) 2012 Robert Wessels. All right reserved.
  *
  *     Contribution: Nathan Crum & Patrick Marcus
  *
  ***********************************************************************
  Derived from:
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

#ifndef Pins_Energia_h
#define Pins_Energia_h
#ifndef BV
#define BV(x) (1 << (x))
#endif


#if defined(__MSP430_HAS_EUSCI_B0__)
static const uint8_t TWISDA  = 19;  /* P1.6 */
static const uint8_t TWISCL  = 21;  /* P1.7 */
#define TWISDA_SET_MODE  (PORT_SELECTION1 | INPUT_PULLUP)
#define TWISCL_SET_MODE  (PORT_SELECTION1 | INPUT_PULLUP)
#endif

#if defined(__MSP430_HAS_EUSCI_A0__) || defined(__MSP430_HAS_EUSCI_A1__)
static const uint8_t DEBUG_UARTRXD = 0;  	/* Receive  Data (RXD) at P2.1 */
static const uint8_t DEBUG_UARTTXD = 1;  	/* Transmit Data (TXD) at P2.0 */
static const uint8_t AUX_UARTRXD = 12;  	/* Receive  Data (RXD) at P2.6 */
static const uint8_t AUX_UARTTXD = 11;  	/* Transmit Data (TXD) at P2.5 */
#define DEBUG_UARTRXD_SET_MODE (PORT_SELECTION1 | INPUT)
#define DEBUG_UARTTXD_SET_MODE (PORT_SELECTION1 | OUTPUT)
#define AUX_UARTRXD_SET_MODE (PORT_SELECTION1 | INPUT)
#define AUX_UARTTXD_SET_MODE (PORT_SELECTION1 | OUTPUT)
#define DEBUG_UART_MODULE_OFFSET 0x00
#define AUX_UART_MODULE_OFFSET 0x20
#define SERIAL1_AVAILABLE 1
static const uint8_t SS      = 10;   /* P2.3 */
static const uint8_t SCK     = 13;   /* P2.4 */
static const uint8_t MOSI    = 11;  /* P2.5 aka SIMO */
static const uint8_t MISO    = 12;  /* P2.6 aka SOMI */
#define SPISCK_SET_MODE  (PORT_SELECTION1)
#define SPIMOSI_SET_MODE (PORT_SELECTION1)
#define SPIMISO_SET_MODE (PORT_SELECTION1)
#endif


/* Analog pins */

static const uint8_t A0  = 14;
static const uint8_t A1  = 15;
static const uint8_t A2  = 16;
static const uint8_t A3  = 17;
static const uint8_t A4  = 18;
static const uint8_t A5  = 20;
static const uint8_t A6  = 128 + 1; 	// Not available on Shinobi Core
static const uint8_t A7  = 128 + 2; 	// Not available on Shinobi Core
static const uint8_t A8  = 128 + 3; 	// Not available on Shinobi Core
static const uint8_t A9  = 128 + 4; 	// Not available on Shinobi Core
static const uint8_t A10 = 128 + 5;	// Not available on Shinobi Core
static const uint8_t A11  = 128 + 6;	// Not available on Shinobi Core
static const uint8_t A12  = 128 + 7;	// Not available on Shinobi Core
static const uint8_t A13  = 128 + 8;	// Not available on Shinobi Core
static const uint8_t A14  = 128 + 9;	// Not available on Shinobi Core
static const uint8_t A15  = 128 + 10;	// Not available on Shinobi Core

//-------------------------------------------

/* Layout of the 2 - 13 pin headers.

   All pins support digitalWrite() and attachInterrupt()
   Pins marked with PWM support anaglogWrite()
   Pins marked with ADx support analogRead()

															+---\_/---+
													VCC0	|         		|22  P1.1   (AREF)
(PWM)			(SPI-SLK)	    (D13)  P2.4 13	|  STORM  	|14  P3.0	(AD0)
(PWM)			(SPI-CS)	    (D10)  P2.3 10	|  	NINJA  	|15  P3.1	(AD1)
(PWM)			    				(D09)  P1.3 09	|         		|16  P3.2	(AD2)
(PWM)			    				(D08)  P1.2 08	|         		|17  P3.3	(AD3)
(PWM)                  			    (D07)  P1.0 07	|         		|18  P1.4	(AD4)	(SDA)	P1.6		19
(PWM)                     		  	(D06)  P3.7 06	|         		|20  P1.5	(AD5)	(SCL)		P1.7		21
(PWM)                       		(D05)  P3.6 05	|  	_____  	|11  P2.5 	(D11) 	(MOSI) (PWM) (UCA1RXD)
(PWM)                       		(D04)  P3.5 04	|  / 	     \ 	|12  P2.6 	(D12) 	(MISO) (PWM) (UCA1TXD)
(PWM)                      			(D03)  P3.4 03	| |CR2032|	| TST
(PWM)                      			(D02)  P2.2 02	| |       	  |	| RST
(PWM) (UCA0TXD -> Serial1) (D01)  P2.0 01	|  \_____ / | GND
(PWM) (UCA0RXD -> Serial1) (D00)  P2.1 00	|         		| N/C 
															+---------+
*/
// Pin names based on the silkscreen
//

static const uint8_t P1_0 = 7;
static const uint8_t P1_1 = 22;
static const uint8_t P1_2 = 8;
static const uint8_t P1_3 = 9;
static const uint8_t P1_4 = 18;
static const uint8_t P1_5 = 20;
static const uint8_t P1_6 = 19;
static const uint8_t P1_7 = 21;

static const uint8_t P2_0 = 1;
static const uint8_t P2_1 = 0;
static const uint8_t P2_2 = 2;
static const uint8_t P2_3 = 10;
static const uint8_t P2_4 = 13;
static const uint8_t P2_5 = 11;
static const uint8_t P2_6 = 12;
//static const uint8_t P2_7 = ;

static const uint8_t P3_0 = 14;
static const uint8_t P3_1 = 15;
static const uint8_t P3_2 = 16;
static const uint8_t P3_3 = 17;
static const uint8_t P3_4 = 3;
static const uint8_t P3_5 = 4;
static const uint8_t P3_6 = 5;
static const uint8_t P3_7 = 6;

//static const uint8_t P4_0 = ;
//static const uint8_t P4_1 = ;
//static const uint8_t P4_2 = ;
//static const uint8_t P4_3 = ;
static const uint8_t P4_4 = 23;
static const uint8_t P4_5 = 24;
//static const uint8_t P4_6 = ;
//static const uint8_t P4_7 = ;

//static const uint8_t PJ_0 = ;
//static const uint8_t PJ_1 = ;
//static const uint8_t PJ_2 = ;
//static const uint8_t PJ_3 = ;
//static const uint8_t PJ_4 = ;
//static const uint8_t PJ_5 = ;
//static const uint8_t PJ_6 = ;
//static const uint8_t PJ_7 = ;


static const uint8_t LED1 = 23;
static const uint8_t LED2 = 24;
/* For LaunchPad compatability */
static const uint8_t RED_LED = 23;
static const uint8_t GREEN_LED = 24;

//static const uint8_t PUSH1 = ;
//static const uint8_t PUSH2 = ;
static const uint8_t TEMPSENSOR = 1; // depends on chip

#ifdef ARDUINO_MAIN

const uint16_t port_to_input[] = {
	NOT_A_PORT,
	(uint16_t) &P1IN,
	(uint16_t) &P2IN,
#ifdef __MSP430_HAS_PORT3_R__
	(uint16_t) &P3IN,
#endif
#ifdef __MSP430_HAS_PORT4_R__
	(uint16_t) &P4IN,
#endif
#ifdef __MSP430_HAS_PORTJ_R__
	(uint16_t) &PJIN,
#endif
};

const uint16_t port_to_output[] = {
	NOT_A_PORT,
	(uint16_t) &P1OUT,
	(uint16_t) &P2OUT,
#ifdef __MSP430_HAS_PORT3_R__
	(uint16_t) &P3OUT,
#endif
#ifdef __MSP430_HAS_PORT4_R__
	(uint16_t) &P4OUT,
#endif
#ifdef __MSP430_HAS_PORTJ_R__
	(uint16_t) &PJOUT,
#endif
};

const uint16_t port_to_dir[] = {
	NOT_A_PORT,
	(uint16_t) &P1DIR,
	(uint16_t) &P2DIR,
#ifdef __MSP430_HAS_PORT3_R__
	(uint16_t) &P3DIR,
#endif
#ifdef __MSP430_HAS_PORT4_R__
	(uint16_t) &P4DIR,
#endif
#ifdef __MSP430_HAS_PORTJ_R__
	(uint16_t) &PJDIR,
#endif
};

const uint16_t port_to_ren[] = {
	NOT_A_PORT,
	(uint16_t) &P1REN,
	(uint16_t) &P2REN,
#ifdef __MSP430_HAS_PORT3_R__
	(uint16_t) &P3REN,
#endif
#ifdef __MSP430_HAS_PORT4_R__
	(uint16_t) &P4REN,
#endif
#ifdef __MSP430_HAS_PORTJ_R__
	(uint16_t) &PJREN,
#endif
};

const uint16_t port_to_sel0[] = {
	NOT_A_PORT,
	(uint16_t) &P1SEL0,
	(uint16_t) &P2SEL0,
#ifdef __MSP430_HAS_PORT3_R__
	(uint16_t) &P3SEL0,
#endif
#ifdef __MSP430_HAS_PORT4_R__
	(uint16_t) &P4SEL0,
#endif
#ifdef __MSP430_HAS_PORTJ_R__
	(uint16_t) &PJSEL0,
#endif
};

const uint16_t port_to_sel1[] = {
	NOT_A_PORT,
	(uint16_t) &P1SEL1,
	(uint16_t) &P2SEL1,
#ifdef __MSP430_HAS_PORT3_R__
	(uint16_t) &P3SEL1,
#endif
#ifdef __MSP430_HAS_PORT4_R__
	(uint16_t) &P4SEL1,
#endif
#ifdef __MSP430_HAS_PORTJ_R__
	(uint16_t) &PJSEL1,
#endif
};

const uint8_t digital_pin_to_timer[] = {
	T0B0,				/* 00 - P2.1 */
	T0B6,				/* 01 - P2.0 */
	T0B2,				/* 02 - P2.2 */
	T0B3,				/* 03 - P3.4 */
	T0B4,				/* 04 - P3.5 */
	T0B5,				/* 05 - P3.6 */
	T0B6,				/* 06 - P3.7 */
	T0A1,				/* 07 - P1.0 */
	T1A1,				/* 08 - P1.2 */
	T1A2,				/* 09 - P1.3 */
	T0A0,				/* 10 - P2.3 */
	T0B0,				/* 11 - P2.5 */
	T0B1,  				/* 12 - P2.6 */
	T1A0,				/* 13 - P2.4 */
	NOT_ON_TIMER,	/* 14 - P3.0 */
	NOT_ON_TIMER,	/* 15 - P3.1 */
	NOT_ON_TIMER,	/* 16 - P3.2 */
	NOT_ON_TIMER,	/* 17 - P3.3 */
	T0B1,				/* 18 - P1.4 */
	T0B3,				/* 19 - P1.6 */
	T0B2,				/* 20 - P1.5 */
	T0B4,				/* 21 - P1.7 */
	T0A2,				/* 22 - P1.1 */
	T0B5,				/* 23 - P4.4 - RED LED */
	NOT_ON_TIMER,	/* 24 - P4.5 - ORN LED */
};

const uint8_t digital_pin_to_port[] = {
	P2,			/* 00 - P2.1 */
	P2,			/* 01 - P2.0 */
	P2,			/* 02 - P2.2 */
	P3,			/* 03 - P3.4 */
	P3,			/* 04 - P3.5 */
	P3,			/* 05 - P3.6 */
	P3,			/* 06 - P3.7 */
	P1,			/* 07 - P1.0 */
	P1,			/* 08 - P1.2 */
	P1,			/* 09 - P1.3 */
	P2,			/* 10 - P2.3 */
	P2,			/* 11 - P2.5 */
	P2,  			/* 12 - P2.6 */
	P2,			/* 13 - P2.4 */
	P3,			/* 14 - P3.0 */
	P3,			/* 15 - P3.1 */
	P3,			/* 16 - P3.2 */
	P3,			/* 17 - P3.3 */
	P1,			/* 18 - P1.4 */
	P1,			/* 19 - P1.6 */
	P1,			/* 20 - P1.5 */
	P1,			/* 21 - P1.7 */
	P1,			/* 22 - P1.1 */
	P4,			/* 23 - P4.4 - RED LED */
	P4,			/* 24 - P4.5 - ORN LED */
};

const uint8_t digital_pin_to_bit_mask[] = {
	BV(1),			/* 00 - P2.1 */
	BV(0),			/* 01 - P2.0 */
	BV(2),			/* 02 - P2.2 */
	BV(4),			/* 03 - P3.4 */
	BV(5),			/* 04 - P3.5 */
	BV(6),			/* 05 - P3.6 */
	BV(7),			/* 06 - P3.7 */
	BV(0),			/* 07 - P1.0 */
	BV(2),			/* 08 - P1.2 */
	BV(3),			/* 09 - P1.3 */
	BV(3),			/* 10 - P2.3 */
	BV(5),			/* 11 - P2.5 */
	BV(6),  			/* 12 - P2.6 */
	BV(4),			/* 13 - P2.4 */
	BV(0),			/* 14 - P3.0 */
	BV(1),			/* 15 - P3.1 */
	BV(2),			/* 16 - P3.2 */
	BV(3),			/* 17 - P3.3 */
	BV(4),			/* 18 - P1.4 */
	BV(6),			/* 19 - P1.6 */
	BV(5),			/* 20 - P1.5 */
	BV(7),			/* 21 - P1.7 */
	BV(1),			/* 22 - P1.1 */
	BV(4),			/* 23 - P4.4 - RED LED */
	BV(5),			/* 24 - P4.5 - ORN LED */
};

const uint32_t digital_pin_to_analog_in[] = {
	NOT_ON_ADC,		/* 00 - P2.1 */
	NOT_ON_ADC,		/* 01 - P2.0 */
	NOT_ON_ADC,		/* 02 - P2.2 */
	NOT_ON_ADC,		/* 03 - P3.4 */
	NOT_ON_ADC,		/* 04 - P3.5 */
	NOT_ON_ADC,		/* 05 - P3.6 */
	NOT_ON_ADC,		/* 06 - P3.7 */
	NOT_ON_ADC,		/* 07 - P1.0 */
	NOT_ON_ADC,		/* 08 - P1.2 */
	NOT_ON_ADC,		/* 09 - P1.3 */
	NOT_ON_ADC,		/* 10 - P2.3 */
	NOT_ON_ADC,		/* 11 - P2.5 */
	NOT_ON_ADC,  	/* 12 - P2.6 */
	NOT_ON_ADC,		/* 13 - P2.4 */
	0,						/* 14 - P3.0 */
	1,						/* 15 - P3.1 */
	2,						/* 16 - P3.2 */
	3,						/* 17 - P3.3 */
	4,						/* 18 - P1.4 */
	NOT_ON_ADC,		/* 19 - P1.6 */
	5,						/* 20 - P1.5 */
	NOT_ON_ADC,		/* 21 - P1.7 */
	NOT_ON_ADC,		/* 26 - P1.1 */
	NOT_ON_ADC,		/* 27 - P4.4 - RED LED */
	NOT_ON_ADC,		/* 28 - P4.5 - ORN LED */
};
#endif // #ifdef ARDUINO_MAIN
#endif // #ifndef Pins_Energia_h
