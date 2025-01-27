#include "commands.h"
#include "sys_hdr.h"
#include <dos.h>
#include <string.h>
 extern void End_code(void);
 struct BPB_struct bpb;
 uint16_t Media_check_cmd(struct REQ_struct far * r_ptr) 
{
  r_ptr->req_type.media_check_req.return_info = 1;
   return OP_COMPLETE;
}

 uint16_t Build_bpb_cmd(struct REQ_struct far * r_ptr) 
{
  r_ptr->req_type.build_bpb_req.BPB_table = &bpb;
   return OP_COMPLETE;
}

 uint16_t Ioctl_input_cmd(struct REQ_struct far * r_ptr) 
{
  return UNKNOWN_CMD;
}

 uint16_t Input_cmd(struct REQ_struct far * r_ptr) 
{
  r_ptr->req_type.i_o_req.count = 0;
   return OP_COMPLETE;
}

 uint16_t Input_no_wait_cmd(struct REQ_struct far * r_ptr) 
{
  return UNKNOWN_CMD;
}

 uint16_t Input_status_cmd(struct REQ_struct far * r_ptr) 
{
  return UNKNOWN_CMD;
}

 uint16_t Input_flush_cmd(struct REQ_struct far * r_ptr) 
{
  return OP_COMPLETE;
}

 uint16_t Output_cmd(struct REQ_struct far * r_ptr) 
{
  r_ptr->req_type.i_o_req.count = 0;
   return OP_COMPLETE;
}

 uint16_t Output_verify_cmd(struct REQ_struct far * r_ptr) 
{
  r_ptr->req_type.i_o_req.count = 0;
   return OP_COMPLETE;
}

 uint16_t Output_status_cmd(struct REQ_struct far * r_ptr) 
{
  return UNKNOWN_CMD;
}

 uint16_t Output_flush_cmd(struct REQ_struct far * r_ptr) 
{
  return UNKNOWN_CMD;
}

 uint16_t Ioctl_output_cmd(struct REQ_struct far * r_ptr) 
{
  return UNKNOWN_CMD;
}

 uint16_t Dev_open_cmd(struct REQ_struct far * r_ptr) 
{
  return OP_COMPLETE;
}

 uint16_t Dev_close_cmd(struct REQ_struct far * r_ptr) 
{
  return UNKNOWN_CMD;
}

 uint16_t Remove_media_cmd(struct REQ_struct far * r_ptr) 
{
  return UNKNOWN_CMD;
}

 uint16_t Ioctl_cmd(struct REQ_struct far * r_ptr) 
{
  return UNKNOWN_CMD;
}

 uint16_t Get_l_d_map_cmd(struct REQ_struct far * r_ptr) 
{
  r_ptr->req_type.l_d_map_req.unit_code = 0;
   return OP_COMPLETE;
}

 uint16_t Set_l_d_map_cmd(struct REQ_struct far * r_ptr) 
{
  r_ptr->req_type.l_d_map_req.unit_code = 0;
   return OP_COMPLETE;
}

 uint16_t Unknown_cmd(struct REQ_struct far * r_ptr) 
{
  return UNKNOWN_CMD;
}

 
