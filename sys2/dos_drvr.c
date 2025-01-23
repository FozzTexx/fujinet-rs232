#include <dos.h>
#include <string.h>

#include "device.h"

#define DEVICES	1

extern void    End_code(void);

bpb drvr_bpb;
extern bpb *bpb_ary[DEVICES];

unsigned int Media_check_cmd(request far *r_ptr)
{
    r_ptr->_r_x._r_media._r_retcode = 1;

    return S_DONE;
}

unsigned int Build_bpb_cmd(request far *r_ptr)
{
   r_ptr->_r_x._r_bpb._r_bpbpt = &drvr_bpb;

   return S_DONE;
}

unsigned int Ioctl_input_cmd(request far *r_ptr)
{
   return E_CMD;
}

unsigned int Input_cmd(request far *r_ptr)
{
   r_ptr->_r_x._r_rw._r_count = 0;

   return S_DONE;
}

unsigned int Input_no_wait_cmd(request far *r_ptr)
{
   return E_CMD;
}

unsigned int Input_status_cmd(request far *r_ptr)
{
   return E_CMD;
}

unsigned int Input_flush_cmd(request far *r_ptr)
{
   return S_DONE;
}

unsigned int Output_cmd(request far *r_ptr)
{
   r_ptr->_r_x._r_rw._r_count = 0;

   return S_DONE;
}

unsigned int Output_verify_cmd(request far *r_ptr)
{
   r_ptr->_r_x._r_rw._r_count = 0;

   return S_DONE;
}

unsigned int Output_status_cmd(request far *r_ptr)
{
   return E_CMD;
}

unsigned int Output_flush_cmd(request far *r_ptr)
{
   return E_CMD;
}

unsigned int Ioctl_output_cmd(request far *r_ptr)
{
   return E_CMD;
}

unsigned int Dev_open_cmd(request far *r_ptr)
{
   return S_DONE;
}

unsigned int Dev_close_cmd(request far *r_ptr)
{
   return E_CMD;
}

unsigned int Remove_media_cmd(request far *r_ptr)
{
   return E_CMD;
}

unsigned int Ioctl_cmd(request far *r_ptr)
{
   return E_CMD;
}

unsigned int Get_l_d_map_cmd(request far *r_ptr)
{
   r_ptr->_r_x._r_gen._r_cat = 0;

   return S_DONE;
}

unsigned int Set_l_d_map_cmd(request far *r_ptr)
{
   r_ptr->_r_x._r_gen._r_cat = 0;

   return S_DONE;
}

unsigned int Unknown_cmd(request far *r_ptr)
{
   return E_CMD;
}

