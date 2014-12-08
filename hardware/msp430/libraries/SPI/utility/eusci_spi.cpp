/**
 * File: eusci_spi.c - msp430 USCI SPI implementation
 *
 * EUSCI flavor implementation by Robert Wessels <robertinant@yahoo.com>
 * Copyright (c) 2012 by Rick Kimball <rick@kimballsoftware.com>
 * spi abstraction api for msp430
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 *
 */

#include <msp430.h>
#include <stdint.h>
#include "spi_430.h"

#ifdef __MSP430_HAS_EUSCI_B0__

/**
 * USCI flags for various the SPI MODEs
 *
 * Note: The msp430 UCCKPL tracks the CPOL value. However,
 * the UCCKPH flag is inverted when compared to the CPHA
 * value described in Motorola documentation.
 */

#define SPI_MODE_0 (UCCKPH)				/* CPOL=0 CPHA=0 */
#define SPI_MODE_1 (0)					/* CPOL=0 CPHA=1 */
#define SPI_MODE_2 (UCCKPL | UCCKPH)	/* CPOL=1 CPHA=0 */
#define SPI_MODE_3 (UCCKPL)				/* CPOL=1 CPHA=1 */

#define SPI_MODE_MASK (UCCKPL | UCCKPH)

/**
 * spi_initialize() - Configure USCI UCA1 for SPI mode
 *
 * P2.0 - CS (active low)
 * P1.5 - SCLK
 * P1.6 - MISO aka SOMI
 * P1.7 - MOSI aka SIMO
 *
 */
void spi_initialize(void)
{
	/* Put USCI in reset mode, source USCI clock from SMCLK. */
	UCA1CTLW0 = UCSWRST | UCSSEL_2;

	/* SPI in master MODE 0 - CPOL=0 SPHA=0. */
	UCA1CTLW0 |= SPI_MODE_0 | UCMSB | UCSYNC | UCMST;

	/* P2.4 as CLK, P2.5 as MOSI, P2.6 as MISO */
	P2SEL1 |= BIT4 | BIT5 | BIT6;

	/* Set initial speed to 4MHz. */
	UCA1BR0 = SPI_CLOCK_DIV4 & 0xFF;
	UCA1BR1 = (SPI_CLOCK_DIV4 >> 8 ) & 0xFF;

	/* Release USCI for operation. */
	UCA1CTLW0 &= ~UCSWRST;
}

/**
 * spi_disable() - put USCI into reset mode.
 */
void spi_disable(void)
{
	/* Put USCI in reset mode. */
	UCA1CTLW0 |= UCSWRST;
}

/**
 * spi_send() - send a byte and recv response.
 */
uint8_t spi_send(const uint8_t _data)
{
	/* Wait for previous tx to complete. */
	while (!(UCA1IFG & UCTXIFG))
		;

	/* Setting TXBUF clears the TXIFG flag. */
	UCA1TXBUF = _data;

	/* Wait for a rx character? */
	while (!(UCA1IFG & UCRXIFG))
		;

	/* Reading clears RXIFG flag. */
	return UCA1RXBUF;
}

/***SPI_MODE_0
 * spi_set_divisor() - set new clock divider for USCI.
 *
 * USCI speed is based on the SMCLK divided by BR0 and BR1.
 *
 */
void spi_set_divisor(const uint16_t clkdiv)
{
	/* Hold UCA1 in reset. */
	UCA1CTLW0 |= UCSWRST;

	UCA1BR0 = clkdiv & 0xFF;
	UCA1BR1 = (clkdiv >> 8 ) & 0xFF;

	/* Release for operation. */
	UCA1CTLW0 &= ~UCSWRST;
}

/**
 * spi_set_bitorder(LSBFIRST=0 | MSBFIRST=1).
 */
void spi_set_bitorder(const uint8_t order)
{
	/* Hold UCA1 in reset. */
	UCA1CTLW0 |= UCSWRST;

	UCA1CTLW0 = (UCA1CTLW0 & ~UCMSB) | ((order == 1 /*MSBFIRST*/) ? UCMSB : 0); /* MSBFIRST = 1 */

	/* Release for operation. */
	UCA1CTLW0 &= ~UCSWRST;
}

/**
 * spi_set_datamode() - mode 0 - 3.
 */
void spi_set_datamode(const uint8_t mode)
{
	/* Hold UCA1 in reset. */
	UCA1CTL1 |= UCSWRST;
	switch(mode) {
	case 0: /* SPI_MODE0 */
		UCA1CTLW0 = (UCA1CTLW0 & ~SPI_MODE_MASK) | SPI_MODE_0;
		break;
	case 1: /* SPI_MODE1 */
		UCA1CTLW0 = (UCA1CTLW0 & ~SPI_MODE_MASK) | SPI_MODE_1;
		break;
	case 2: /* SPI_MODE2 */
		UCA1CTLW0 = (UCA1CTLW0 & ~SPI_MODE_MASK) | SPI_MODE_2;
		break;
	case 4: /* SPI_MODE3 */
		UCA1CTLW0 = (UCA1CTLW0 & ~SPI_MODE_MASK) | SPI_MODE_3;
		break;
	default:
		break;
	}

	/* Release for operation. */
	UCA1CTL1 &= ~UCSWRST;
}
#endif
