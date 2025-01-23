#include "dos_dd.h"
#include "com.h"

#if 0
extern void far Strategy();
extern void far Interrupt();

struct DEVICE_HEADER_struct dos_header =
{
    (struct DEVICE_HEADER_struct far *) 0xFFFFFFFFL,
    0x8003, /* Chr,  STDIN, STDOUT */
    (unsigned int) Strategy,
    (unsigned int) Interrupt,
    {'F','U','J','I',' ',' ',' ',' '}
};
#endif

struct  BPB_struct      bpb = 
{
   512,
   1,
   1,
   2,
   64,
   360,
   0xF0,
   2,
   1,
   1,
   1L,
   0L
};

struct  BPB_struct      *bpb_ary[DEVICES] = { 0 };

unsigned int    rc;     /* Function Return code */
unsigned int    driver; /* Global driver variable */
unsigned int    SS_reg; /* SS Register Variable */
unsigned int    SP_reg; /* SP Register Variable */
unsigned int    ES_reg; /* ES Register Variable */
unsigned int    AX_reg; /* AX Register Variable */
unsigned int    BX_reg; /* BX Register Variable */
unsigned int    CX_reg; /* CX Register Variable */
unsigned int    DX_reg; /* DX Register Variable */
unsigned int    DS_reg; /* DS Register Variable */
unsigned int    SI_reg; /* SI Register Variable */

unsigned int    local_stk[STK_SIZE];

PORT port;

struct REQ_struct       far *r_ptr;
