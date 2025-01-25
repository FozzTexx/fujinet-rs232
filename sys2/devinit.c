/*
 * Template for writing DOS device drivers in Open Watcom C
 *
 * Copyright (C) 2022, Eduardo Casino (mail@eduardocasino.es)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 *
 */

#include <dos.h>
#include <stdint.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

#include "devinit.h"
#include "fujinet.h"
#include "fujicom.h"
#include "com.h"

#pragma data_seg("_CODE")

PORT my_port;
cmdFrame_t __interrupt c;

struct _tm
{
	char tm_mday;
	char tm_month;
	char tm_year;
	char tm_hour;
	char tm_min;
	char tm_sec;
};

//
// Place here any variables or constants that should go away after initialization
//
static char hellomsg[] = "\r\FujiNet in Open Watcom C\r\n$";

void byte_to_hex(char *buffer, unsigned char byte)
{
  const char hex_digits[] = "0123456789ABCDEF";
  buffer[0] = hex_digits[(byte >> 4) & 0xF]; // High nibble
  buffer[1] = hex_digits[byte & 0xF];        // Low nibble
  return;
}

uint16_t Init_cmd( void )
{
  printMsg(hellomsg);

  fpRequest->r_endaddr = MK_FP(getCS(), &transient_data);

#if 0
  {
    unsigned int val;
    PORT *port;


#if 0
    port = port_open(0x3f8, 12);
#else
    port = port_open_static(&my_port, 0x3f8, 12);
#endif
    strcpy(hellomsg, "ADDR  0x0000\r\n$");
    byte_to_hex(&hellomsg[8], ((unsigned int) port) >> 8);
    byte_to_hex(&hellomsg[10], (unsigned int) port);
    printMsg(hellomsg);

    strcpy(hellomsg, "IIR   0x00\r\n$");
    val = inp(port->uart_base + 2); // Get IIR
    byte_to_hex(&hellomsg[8], val);
    printMsg(hellomsg);
    
    strcpy(hellomsg, "LINES 0x00\r\n$");

    val = inp(port->uart_base + 6); // Get modem lines
    byte_to_hex(&hellomsg[8], val);
    printMsg(hellomsg);

    outp(port->uart_base + 4, 0x1A);
    val = inp(port->uart_base + 6); // Get modem lines
    byte_to_hex(&hellomsg[8], val);
    printMsg(hellomsg);

    outp(port->uart_base + 4, 0x15);
    val = inp(port->uart_base + 6); // Get modem lines
    byte_to_hex(&hellomsg[8], val);
    printMsg(hellomsg);

    outp(port->uart_base + 4, 0);
    port_set(port,9600,'N',8,1);
    port_put(port, "ABCDEF", 6);
  }
#endif
  
#if 1
  {
    char reply=0;
    union REGS __interrupt r;
    struct _tm t;


    c.ddev = 0x45;
    c.dcomnd = 0x9A;

    printMsg("Serial init\r\n$");
    fujicom_init();
    printMsg("Serial read\r\n$");
    reply = fujicom_command_read(&c,(unsigned char *)&t,sizeof(t));
    printMsg("Serial done\r\n$");

    if (reply != 'C')
      {
	printMsg("Could not read time from FujiNet.\r\nAborted.\r\n$");
	strcpy(hellomsg, "00\r\n$");
	byte_to_hex(hellomsg, reply);
	printMsg(hellomsg);
	fujicom_done();
	return 1;
      }

    r.h.ah = 0x2B;
    r.x.cx = t.tm_year + 2000;
    r.h.dh = t.tm_month;
    r.h.dl = t.tm_mday;

    //intdos(&r,NULL);

    r.h.ah = 0x2D;
    r.h.ch = t.tm_hour;
    r.h.cl = t.tm_min;
    r.h.dh = t.tm_sec;
    r.h.dl = 0;

    //intdos(&r,NULL);

    printMsg("MS-DOS Time now set from FujiNet\r\n$");
    printMsg("DATE: %02u/%02u/%02u\r\n$");//,t.tm_month,t.tm_mday,t.tm_year);
    printMsg("TIME: %02u:%02u:%02u\r\n$");//,t.tm_hour,t.tm_min,t.tm_sec);

  }
#endif

  return 0;
}

void main()
{
}
