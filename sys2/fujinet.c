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

#include <stdint.h>
#include <stddef.h>

#include "device.h"
#include "devinit.h"
#include "fujinet.h"
#include "dos_drvr.h"

#ifdef USE_INTERNAL_STACK

static uint8_t our_stack[STACK_SIZE];
uint8_t *stack_bottom = our_stack + STACK_SIZE;
uint32_t dos_stack;

#endif // USE_INTERNAL_STACK

request __far *fpRequest = (request __far *)0;

static driverFunction_t dispatchTable[] =
{
  Init_cmd,
  Media_check_cmd,
  Build_bpb_cmd,
  Ioctl_input_cmd,
  Input_cmd,
  Input_no_wait_cmd,
  Input_status_cmd,
  Input_flush_cmd,
  Output_cmd,
  Output_verify_cmd,
  Output_status_cmd,
  Output_flush_cmd,
  Ioctl_output_cmd,
  Dev_open_cmd,
  Dev_close_cmd,
  Remove_media_cmd,
  Unknown_cmd,
  Unknown_cmd,
  Unknown_cmd,
  Ioctl_cmd,
  Unknown_cmd,
  Unknown_cmd,
  Unknown_cmd,
  Get_l_d_map_cmd,
  Set_l_d_map_cmd
};

static driverFunction_t currentFunction;

void __far DeviceInterrupt( void )
#pragma aux DeviceInterrupt __parm []
{
#ifdef USE_INTERNAL_STACK
    switch_stack();
#endif

    push_regs();

    if ( fpRequest->r_command > C_MAXCMD || NULL == (currentFunction = dispatchTable[fpRequest->r_command]) )
    {
        fpRequest->r_status = S_DONE | S_ERROR | E_CMD;
    }
    else
    {
        fpRequest->r_status = currentFunction();
    }

    pop_regs();

#ifdef USE_INTERNAL_STACK
    restore_stack();
#endif
}

void __far DeviceStrategy( request __far *req )
#pragma aux DeviceStrategy __parm [__es __bx]
{
    fpRequest = req;
}

