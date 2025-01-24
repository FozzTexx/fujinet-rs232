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

#include "devinit.h"
#include "fujinet.h"
#include "fujicom.h"

#pragma data_seg("_CODE")

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

uint16_t Init_cmd( void )
{
  printMsg(hellomsg);

  fpRequest->r_endaddr = MK_FP(getCS(), &transient_data);

  {
    char reply=0;
    cmdFrame_t __interrupt c;
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

  return 0;
}

void main()
{
}
