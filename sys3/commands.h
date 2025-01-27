#ifndef _COMMANDS_H
#define _COMMANDS_H
#include "sys_hdr.h"
#include <stdint.h>
 extern uint16_t Init_cmd(SYSREQ far * r_ptr);
extern uint16_t Media_check_cmd(SYSREQ far * r_ptr);
extern uint16_t Build_bpb_cmd(SYSREQ far * r_ptr);
extern uint16_t Ioctl_input_cmd(SYSREQ far * r_ptr);
extern uint16_t Input_cmd(SYSREQ far * r_ptr);
extern uint16_t Input_no_wait_cmd(SYSREQ far * r_ptr);
extern uint16_t Input_status_cmd(SYSREQ far * r_ptr);
extern uint16_t Input_flush_cmd(SYSREQ far * r_ptr);
extern uint16_t Output_cmd(SYSREQ far * r_ptr);
extern uint16_t Output_verify_cmd(SYSREQ far * r_ptr);
extern uint16_t Output_status_cmd(SYSREQ far * r_ptr);
extern uint16_t Output_flush_cmd(SYSREQ far * r_ptr);
extern uint16_t Ioctl_output_cmd(SYSREQ far * r_ptr);
extern uint16_t Dev_open_cmd(SYSREQ far * r_ptr);
extern uint16_t Dev_close_cmd(SYSREQ far * r_ptr);
extern uint16_t Remove_media_cmd(SYSREQ far * r_ptr);
extern uint16_t Ioctl_cmd(SYSREQ far * r_ptr);
extern uint16_t Get_l_d_map_cmd(SYSREQ far * r_ptr);
extern uint16_t Set_l_d_map_cmd(SYSREQ far * r_ptr);
extern uint16_t Unknown_cmd(SYSREQ far * r_ptr);



#endif  /* _COMMANDS_H */
