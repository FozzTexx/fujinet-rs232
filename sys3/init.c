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

#undef LOOPBACK_TEST

#include "fujicom.h"
#include "print.h"
#include "dispatch.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <dos.h>

//#pragma data_seg("_CODE")

struct _tm {
  char tm_mday;
  char tm_month;
  char tm_year;
  char tm_hour;
  char tm_min;
  char tm_sec;
};

struct _tm t;
union REGS r;
cmdFrame_t c;

static char hellomsg[] = "\r\FujiNet in Open Watcom C\r\n$";

extern __segment getCS(void);


#pragma aux getCS = \
    "mov ax, cs";

uint16_t Init_cmd(void)
{
  printMsg(hellomsg);

  fpRequest->req_type.init_req.end_ptr = MK_FP(getCS(), &transient_data);

#ifdef LOOPBACK_TEST
  {
    uint16_t val;
    PORT my_port;
    PORT *port;


    port = port_open_static(&my_port, 0x3f8, 12);

    strcpy(hellomsg, "ADDR  0x0000\r\n$");
    byte_to_hex(&hellomsg[8], ((uint16_t) port) >> 8);
    byte_to_hex(&hellomsg[10], (uint16_t) port);
    printMsg(hellomsg);

    strcpy(hellomsg, "IIR   0x00\r\n$");
    val = inp(port->uart_base + 2);     // Get IIR
    byte_to_hex(&hellomsg[8], val);
    printMsg(hellomsg);

    strcpy(hellomsg, "LINES 0x00\r\n$");

    val = inp(port->uart_base + 6);     // Get modem lines
    byte_to_hex(&hellomsg[8], val);
    printMsg(hellomsg);

    outp(port->uart_base + 4, 8 + 0);   //0x0A);
    val = inp(port->uart_base + 6);     // Get modem lines
    byte_to_hex(&hellomsg[8], val);
    printMsg(hellomsg);

    outp(port->uart_base + 4, 8 + 3);   //0x05);
    val = inp(port->uart_base + 6);     // Get modem lines
    byte_to_hex(&hellomsg[8], val);
    printMsg(hellomsg);

    port_set(port, 9600, 'N', 8, 1);
    //outp(port->uart_base + 4, 3 + 8);

    {
      int idx;


      for (idx = 0; test_data[idx]; idx++) {
        outp(port->uart_base, test_data[idx]);
        for (;;) {
          val = inp(port->uart_base + 5);
          if (val & 1)
            break;
#if 0
          strcpy(hellomsg, "LSR: 0x00\r\n$");
          byte_to_hex(&hellomsg[7], val);
          printMsg(hellomsg);
          break;
#endif
        }
        //val = inp(port->uart_base);
        val = port_getc(port);
        strcpy(hellomsg, "RCV: 0x00\r\n$");
        byte_to_hex(&hellomsg[7], val);
        printMsg(hellomsg);
      }
    }
  }
#else /* !LOOPBACK_TEST */
  {
    char reply = 0;


    c.ddev = 0x45;
    c.dcomnd = 0x9A;

    printMsg("Serial init\r\n$");
    fujicom_init();
    printMsg("Serial read\r\n$");
    reply = fujicom_command_read(&c, (uint8_t *) & t, sizeof(t));
    printMsg("Serial done\r\n$");

    if (reply != 'C') {
      printMsg("Could not read time from FujiNet.\r\nAborted.\r\n$");
      fujicom_done();
      return 1;
    }

    r.h.ah = 0x2B;
    r.x.cx = t.tm_year + 2000;
    r.h.dh = t.tm_month;
    r.h.dl = t.tm_mday;

    intdos(&r, NULL);

    r.h.ah = 0x2D;
    r.h.ch = t.tm_hour;
    r.h.cl = t.tm_min;
    r.h.dh = t.tm_sec;
    r.h.dl = 0;

    intdos(&r, NULL);

    printMsg("MS-DOS Time now set from FujiNet\r\n$");
    strcpy(hellomsg, "DATE: 00/00/00\r\n$");
    byte_to_decimal(&hellomsg[6], t.tm_month);
    byte_to_decimal(&hellomsg[9], t.tm_mday);
    byte_to_decimal(&hellomsg[12], t.tm_year);
    printMsg(hellomsg);

    strcpy(hellomsg, "TIME: 00:00:00\r\n$");
    byte_to_decimal(&hellomsg[6], t.tm_hour);
    byte_to_decimal(&hellomsg[9], t.tm_min);
    byte_to_decimal(&hellomsg[12], t.tm_sec);
    printMsg(hellomsg);
  }
#endif /* LOOPBACK_TEST */

  return 0;
}

void main()
{
}
