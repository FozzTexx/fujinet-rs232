#include "commands.h"
#include "sys_hdr.h"
#include <dos.h>
#include <stddef.h>

#define disable() _asm { cli }
#define enable() _asm { sti }
  SYSREQ __far * fpRequest = (SYSREQ __far *) 0;
extern uint16_t local_stk[STK_SIZE];
 extern struct REQ_struct far *r_ptr;
 typedef uint16_t(*driverFunction_t) (SYSREQ far * r_ptr);
 static driverFunction_t currentFunction;


static driverFunction_t dispatchTable[] = 
  { Init_cmd, Media_check_cmd, Build_bpb_cmd, Ioctl_input_cmd, Input_cmd,
Input_no_wait_cmd, Input_status_cmd, Input_flush_cmd, Output_cmd, Output_verify_cmd, Output_status_cmd, Output_flush_cmd,
Ioctl_output_cmd, Dev_open_cmd, Dev_close_cmd, Remove_media_cmd, Unknown_cmd, Unknown_cmd, Unknown_cmd, Ioctl_cmd,
Unknown_cmd, Unknown_cmd, Unknown_cmd, Get_l_d_map_cmd, Set_l_d_map_cmd 
};

 
#if ( _M_IX86 >= 0x200 )
#define push_all "pusha"
#define pop_all "popa"
#else   /*  */
#define push_all "push ax" "push cx" "push dx" "push bx" "push sp" "push bp" "push si" "push di"
#define pop_all "pop di" "pop si" "pop bp" "pop bx" "pop bx" "pop dx" "pop cx" "pop ax"
#endif  /*  */
  
#if ( _M_IX86 >= 0x300 )
#define push_segregs "push ds" "push es" "push fs" "push gs"
#define pop_segregs "pop gs" "pop fs" "pop es" "pop ds"
#else   /*  */
#define push_segregs "push ds" "push es"
#define pop_segregs "pop es" "pop ds"
#endif  /*  */
extern void push_regs(void);



#pragma aux push_regs = \
  "pushf" \ push_all \ push_segregs \ "push cs" "pop ds";
 extern void pop_regs(void);



#pragma aux pop_regs = \
  pop_segregs \ pop_all \ "popf";
 void far Strategy(SYSREQ far * r_ptr) 
#pragma aux Strategy __parm [__es __bx]
{
  fpRequest = r_ptr;
  return;
}

 void far Interrupt(void) 
#pragma aux Interrupt __parm []
{
  push_regs();
   if (fpRequest->command > MAXCOMMAND 
         ||!(currentFunction = dispatchTable[fpRequest->command]))
     {
    fpRequest->status = DONE_BIT | ERROR_BIT | UNKNOWN_CMD;
    }
  
  else
     {
    fpRequest->status = currentFunction(NULL);
    }
   pop_regs();
  return;
}


