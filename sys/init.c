#undef WITHOUT_GLOBAL
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

#ifndef WITHOUT_GLOBAL
cmdFrame_t cmd;
#endif /* WITHOUT_GLOBAL */

union REGS iregs;

static char hellomsg[] = "\r\FujiNet in Open Watcom C\r\n$";

extern __segment getCS(void);

#pragma aux getCS = \
    "mov ax, cs";

uint16_t Init_cmd(void)
{
  printDTerm(hellomsg);

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
    printDTerm(hellomsg);

    strcpy(hellomsg, "IIR   0x00\r\n$");
    val = inp(port->uart_base + 2);     // Get IIR
    byte_to_hex(&hellomsg[8], val);
    printDTerm(hellomsg);

    strcpy(hellomsg, "LINES 0x00\r\n$");

    val = inp(port->uart_base + 6);     // Get modem lines
    byte_to_hex(&hellomsg[8], val);
    printDTerm(hellomsg);

    outp(port->uart_base + 4, 8 + 0);   //0x0A);
    val = inp(port->uart_base + 6);     // Get modem lines
    byte_to_hex(&hellomsg[8], val);
    printDTerm(hellomsg);

    outp(port->uart_base + 4, 8 + 3);   //0x05);
    val = inp(port->uart_base + 6);     // Get modem lines
    byte_to_hex(&hellomsg[8], val);
    printDTerm(hellomsg);

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
          printDTerm(hellomsg);
          break;
#endif
        }
        //val = inp(port->uart_base);
        val = port_getc(port);
        strcpy(hellomsg, "RCV: 0x00\r\n$");
        byte_to_hex(&hellomsg[7], val);
        printDTerm(hellomsg);
      }
    }
  }
#else /* !LOOPBACK_TEST */
  {
    char reply = 0;
    struct _tm cur_time;
#ifdef WITHOUT_GLOBAL
    cmdFrame_t cmd;
#endif /* WITHOUT_GLOBAL */


    cmd.ddev = 0x45;
    cmd.dcomnd = 0x9A;

    fujicom_init();
    reply = fujicom_command_read(&cmd, (uint8_t *) &cur_time, sizeof(cur_time));

    if (reply != 'C') {
      printDTerm("Could not read time from FujiNet.\r\nAborted.\r\n$");
      fujicom_done();
      return 1;
    }

    iregs.h.ah = 0x2B;
    iregs.x.cx = cur_time.tm_year + 2000;
    iregs.h.dh = cur_time.tm_month;
    iregs.h.dl = cur_time.tm_mday;

    intdos(&iregs, NULL);

    iregs.h.ah = 0x2D;
    iregs.h.ch = cur_time.tm_hour;
    iregs.h.cl = cur_time.tm_min;
    iregs.h.dh = cur_time.tm_sec;
    iregs.h.dl = 0;

    intdos(&iregs, NULL);

    printDTerm("MS-DOS Time now set from FujiNet\r\n$");
    strcpy(hellomsg, "DATE: 00/00/00\r\n$");
    byte_to_decimal(&hellomsg[6], cur_time.tm_month);
    byte_to_decimal(&hellomsg[9], cur_time.tm_mday);
    byte_to_decimal(&hellomsg[12], cur_time.tm_year);
    printDTerm(hellomsg);

    strcpy(hellomsg, "TIME: 00:00:00\r\n$");
    byte_to_decimal(&hellomsg[6], cur_time.tm_hour);
    byte_to_decimal(&hellomsg[9], cur_time.tm_min);
    byte_to_decimal(&hellomsg[12], cur_time.tm_sec);
    printDTerm(hellomsg);
  }
#endif /* LOOPBACK_TEST */

  return 0;
}
