#include <dos.h>
#include "dos_dd.h"

#define disable() _asm { cli }
#define enable() _asm { sti }

extern	unsigned short Init_cmd();		/* INIT Command */
extern	unsigned short Media_check_cmd();	/* Media Check Command */
extern  unsigned short Build_bpb_cmd();       /* Build BPB Command */
extern	unsigned short Ioctl_input_cmd();	/* IOCTL Input Command */
extern	unsigned short Input_cmd();		/* Input Command */
extern	unsigned short Input_no_wait_cmd();	/* Input No Wait Command */
extern	unsigned short Input_status_cmd();	/* Input Status Command */
extern	unsigned short Input_flush_cmd();	/* Input Flush Command */
extern	unsigned short Output_cmd();		/* Output Command */
extern	unsigned short Output_verify_cmd();	/* Output Verify Command */
extern	unsigned short Output_status_cmd();	/* Output Status Command */
extern	unsigned short Output_flush_cmd();	/* Output Flush Command */
extern	unsigned short Ioctl_output_cmd();	/* IOCTL Output Command */
extern	unsigned short Dev_open_cmd();	/* Device Open Command */
extern  unsigned short Dev_close_cmd();	/* Device Close Command */
extern	unsigned short Remove_media_cmd();	/* Remove media command */
extern	unsigned short Ioctl_cmd();		/* GENERIC IOCTL Command */
extern	unsigned short Get_l_d_map_cmd();	/* GET Logical Device Map */
extern	unsigned short Set_l_d_map_cmd();	/* SET Logical Device Map */
extern  unsigned short Unknown_cmd();		/* UNKNOWN Command Default */

extern	unsigned short rc;			/* Function Return Code */
extern  unsigned short driver;		/* Global Driver Variable */
extern  unsigned short SS_reg;		/* SS Register Variable */
extern  unsigned short SP_reg;		/* SP Register Variable */
extern  unsigned short ES_reg;		/* ES Register Variable */
extern	unsigned short AX_reg;		/* AX Register Variable */
extern	unsigned short BX_reg;		/* BX Register Variable */
extern	unsigned short CX_reg;		/* CX Register Variable */
extern	unsigned short DX_reg;		/* DX Register Variable */
extern	unsigned short DS_reg;		/* DS Register Variable */
extern  unsigned short SI_reg;		/* SI Register Variable */

extern	unsigned short local_stk[STK_SIZE];

extern	struct REQ_struct far *r_ptr;

unsigned short (*dos_cmd[DOS_CMDS]) (struct REQ_struct far *r_ptr) =
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

void DOS_Setup (unsigned short int which,
		unsigned short int ES_tmp,
		unsigned short int DS_tmp,
		unsigned short int AX_tmp)
{
  _asm {
    mov ax, cs;        // Obtain Code Segment
    mov ds, ax;        // Setup Data Segment
  }

  _asm {
    mov BX_reg, bx;    // Save BX
    mov CX_reg, cx;    // Save CX
    mov DX_reg, dx;    // Save DX
  }

  AX_reg = AX_tmp;		/* Save AX */
  ES_reg = ES_tmp;		/* Save Request Pointer */

  _asm {
    mov SS_reg, ss;    // Save Stack Segment
    mov SP_reg, sp;    // Save Stack Pointer
  }

  driver = which;		/* Move value from stack */
  disable();			/* Disable Interrupts */

  _asm {
    mov ax, ds;        // Obtain Data Segment
    mov ss, ax;        // Setup new stack segment
    lea sp, local_stk[STK_SIZE];  // Set stack pointer to top of local_stk
  }

  enable();			/* Enable Interrupts */

  if (driver)
    {
      rc = 0x0000;		/* Clear return code */

      r_ptr = MK_FP(ES_reg, BX_reg);

      if (r_ptr->command >= DOS_CMDS)
	{
	  rc = ERROR_BIT | UNKNOWN_CMD; /* return error */
	}
      else
	{
	  rc |= (*dos_cmd[r_ptr->command])(r_ptr);
	}

      r_ptr->status = rc | DONE_BIT; /* return done */
    }
  else
    {
      /* Why is this empty ? */
    }

  disable();
  _asm {
    mov ss, SS_reg;     // Restore stack segment
    mov sp, SP_reg;     // Restore stack pointer
  }
  enable();
  _asm {
    mov dx, DX_reg;     // Restore DX register
    mov cx, CX_reg;     // Restore CX register
    mov bx, BX_reg;     // Restore BX register
    mov ax, AX_reg;     // Restore AX register

    mov es, ES_tmp;     // Restore ES register
    mov ds, DS_tmp;     // Restore DS register
  }
}

void far Strategy(void)
{
#ifdef DEBUG
  geninterrupt(0x03); /* Break to debugger */
#endif /* DEBUG */

  unsigned short es_val, ds_val, ax_val;

  _asm {
    mov es_val, es;   // Store ES register value in variable
    mov ds_val, ds;   // Store DS register value in variable
    mov ax_val, ax;   // Store AX register value in variable
  }

  DOS_Setup(0x00, es_val, ds_val, ax_val);
}

void far Interrupt(void)
{
#ifdef DEBUG
  geninterrupt(0x03);
#endif /* DEBUG */

  unsigned short es_val, ds_val, ax_val;

  _asm {
    mov es_val, es;   // Store ES register value in variable
    mov ds_val, ds;   // Store DS register value in variable
    mov ax_val, ax;   // Store AX register value in variable
  }

  DOS_Setup(0x01, es_val, ds_val, ax_val);
}
