// Implementation of the interface described in uart.h.

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <joerg@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.        Joerg Wunsch
 * ----------------------------------------------------------------------------
 *
 * Stdio demo, UART implementation of interface described in uart.h
 *
 * $Id: uart.c 1008 2005-12-28 21:38:59Z joerg_wunsch $
 */

#include <avr/io.h>

#include "uart.h"

void
uart_init (void)
{
  // FIXME: is there some hardware that we should be waking from sleep here?
  // in general is automatic wake/sleep something we want to do?  I sort of
  // think not but maybe it's realistic

#ifndef F_CPU
#  error the AVR libc util/setbaud.h header will require F_CPU to be defined
#endif

  // The magical AVR libc util/setbaud.h header requires BAUD to be defined,
  // so we first ensure it isn't already defined then set it.
#ifdef BAUD
#  error We need to set BAUD, but it's already defined, so we're too scared
#endif
#define BAUD UART_BAUD

// This is a special calculation-only header that can be included anywhere.
// It's going to give us back some macros that help set up the serial port
// control registers: UBRRH_VALUE, UBRRL_VALUE, and USE_2X.
#include <util/setbaud.h>

  // FIXME: weirdness: UCSR0A user read-modify-write, while UCSR0B is
  // explicitly set.  Nothing else in UCSR0B is wanted in this implementation,
  // but it's weird to read-modify-write UCSR0A, and slightly weird not
  // to mention the desired default 0 values for the UCSR0B bits.  This is
  // especially the case because many of the other bits of UCSR0A are almost
  // certainly invalidated by a uart_init() (e.g. RXC0, TXC0, FE0, DOR0),
  // in fact it is perhaps because of these bits that we may be supposed to
  // re-init the uart after sleep, if in nfact we are supposed to do that
  // (haven't found the part of the datasheet that say's we're supposed
  // to yet).

  // Set up clocking
  UBRR0L = UBRRL_VALUE;
  UBRR0H = UBRRH_VALUE;
#if USE_2X
  UCSR0A |= _BV (U2X0);
#else
  UCSR0A &= ~(_BV (U2X0));
#endif

  UCSR0B = _BV (TXEN0) | _BV (RXEN0);   // Enable TX/RX
}
