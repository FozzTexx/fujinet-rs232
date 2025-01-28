/*
 *  Listing 2  -  COM.H  -  "RS-232 Interrupts The C Way"
 *
 *  Copyright (C) Mark R. Nelson 1990
 *
 * This file contains the structure definitions, constants,
 * and function prototypes needed to use the RS-232
 * interface code supplied in COM.C.  It should be included
 * by any routine using COM.C procedures.
 */

#ifndef _COM_H
#define _COM_H

#include <stdint.h>

/*
 * This structure defines the 256 byte buffer used for
 * I/O buffers by the COM routines.  By using a buffer
 * size of 256 bytes, updating the indices is simplified.
 */
typedef struct {
  char buffer[256];
  uint8_t write_index;
  uint8_t read_index;
} BUFFER;

/*
 * This structure defines a COM port.  It is initialized
 * when the port is opened with port_open().
 */
typedef struct {
  void (interrupt far * old_vector) ();
  int uart_base;
  int irq_mask;
  int interrupt_number;
  BUFFER in;
  BUFFER out;
} PORT;

/*
 * the ifdef M_186 is checking for Microsoft C/QuickC.
 * Borland and Microsoft differ slightly on the names of
 * some of the DOS specific procedure names, and the
 * fixing up is done here.
 */
#ifdef M_I86
#define inportb inp
#define outportb outp
#define getvect _dos_getvect
#define setvect _dos_setvect
#define enable _enable
#endif /*  */
/*
 * The fully qualified function prototypes.  All of these
 * routines actually reside in COM.C
 */
PORT *port_open(int address, int interrupt_number);
PORT *port_open_static(PORT * port, int address, int interrupt_number);
void port_set(PORT * port, long speed, char parity, int data, int stopbits);
void port_close(PORT * port);
void port_close_static(PORT * port);
int port_putc(uint8_t c, PORT * port);
int port_available(PORT * port);
int port_getc(PORT * port);
int port_getc_sync(PORT * port, uint16_t timeout);
void port_set_dtr(PORT * port, uint8_t t);
void port_put(PORT * port, uint8_t * buf, uint16_t len);

/*
 * These are the standard UART addresses and interrupt
 * numbers for the two IBM compatible COM ports.
 */
#define COM1_UART         0x3f8
#define COM1_INTERRUPT    12
#define COM2_UART         0x2f8
#define COM2_INTERRUPT    11

#endif /* _COM_H */
