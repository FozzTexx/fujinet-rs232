#include "dos_dd.h"
#include "video.h"
#include <dos.h>
#include <string.h>

extern void    End_code(void);

extern struct  BPB_struct  bpb;
extern struct  BPB_struct *bpb_ary[DEVICES];

unsigned int Init_cmd(struct REQ_struct far *r_ptr)
{
    Write_tty("#FUJINET Network Driver 0.0\r\n");
    r_ptr->req_type.init_req.end_ptr =
      MK_FP(FP_SEG(End_code),(unsigned int)End_code);

    return OP_COMPLETE;
}

unsigned int Media_check_cmd(struct REQ_struct far *r_ptr)
{
    r_ptr->req_type.media_check_req.return_info = 1;

    return OP_COMPLETE;
}

unsigned int Build_bpb_cmd(struct REQ_struct far *r_ptr)
{
   r_ptr->req_type.build_bpb_req.BPB_table = &bpb;

   return OP_COMPLETE;
}

unsigned int Ioctl_input_cmd(struct REQ_struct far *r_ptr)
{
   return UNKNOWN_CMD;
}

unsigned int Input_cmd(struct REQ_struct far *r_ptr)
{
   r_ptr->req_type.i_o_req.count = 0;

   return OP_COMPLETE;
}

unsigned int Input_no_wait_cmd(struct REQ_struct far *r_ptr)
{
   return UNKNOWN_CMD;
}

unsigned int Input_status_cmd(struct REQ_struct far *r_ptr)
{
   return UNKNOWN_CMD;
}

unsigned int Input_flush_cmd(struct REQ_struct far *r_ptr)
{
   return OP_COMPLETE;
}

unsigned int Output_cmd(struct REQ_struct far *r_ptr)
{
   r_ptr->req_type.i_o_req.count = 0;

   return OP_COMPLETE;
}

unsigned int Output_verify_cmd(struct REQ_struct far *r_ptr)
{
   r_ptr->req_type.i_o_req.count = 0;

   return OP_COMPLETE;
}

unsigned int Output_status_cmd(struct REQ_struct far *r_ptr)
{
   return UNKNOWN_CMD;
}

unsigned int Output_flush_cmd(struct REQ_struct far *r_ptr)
{
   return UNKNOWN_CMD;
}

unsigned int Ioctl_output_cmd(struct REQ_struct far *r_ptr)
{
   return UNKNOWN_CMD;
}

unsigned int Dev_open_cmd(struct REQ_struct far *r_ptr)
{
   return OP_COMPLETE;
}

unsigned int Dev_close_cmd(struct REQ_struct far *r_ptr)
{
   return UNKNOWN_CMD;
}

unsigned int Remove_media_cmd(struct REQ_struct far *r_ptr)
{
   return UNKNOWN_CMD;
}

unsigned int Ioctl_cmd(struct REQ_struct far *r_ptr)
{
   return UNKNOWN_CMD;
}

unsigned int Get_l_d_map_cmd(struct REQ_struct far *r_ptr)
{
   r_ptr->req_type.l_d_map_req.unit_code = 0;

   return OP_COMPLETE;
}

unsigned int Set_l_d_map_cmd(struct REQ_struct far *r_ptr)
{
   r_ptr->req_type.l_d_map_req.unit_code = 0;

   return OP_COMPLETE;
}

unsigned int Unknown_cmd(struct REQ_struct far *r_ptr)
{
   return UNKNOWN_CMD;
}

