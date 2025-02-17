#include "commands.h"
#include "fujinet.h"
#include "sys_hdr.h"
#include "fujicom.h"
#include "print.h"
#include <string.h>
#include <dos.h>

#undef DEBUG

#define SECTOR_SIZE	512

extern void End_code(void);

struct BPB_struct fn_bpb_table[FN_MAX_DEV];
struct BPB_struct *fn_bpb_pointers[FN_MAX_DEV + 1]; // leave room for the NULL terminator

static uint8_t sector_buf[SECTOR_SIZE];
static cmdFrame_t cmd; // FIXME - make this shared with init.c?
static struct BPB_struct active_bpb[FN_MAX_DEV];

uint16_t Media_check_cmd(SYSREQ far *req)
{
  // FIXME - query FujiNet and see if disk was changed
  req->req_type.media_check_req.return_info = 0;
  return OP_COMPLETE;
}

uint16_t Build_bpb_cmd(SYSREQ far *req)
{
  char reply;
  uint8_t far *buf;


  if (req->unit > FN_MAX_DEV) {
    consolef("Invalid BPB unit: %i\n", req->unit);
    return ERROR_BIT;
  }

  cmd.device = DEVICEID_DISK;  /* Drive 1 */
  cmd.comnd = 'R'; /* Read */
  cmd.aux1 = cmd.aux2 = 0;

  // DOS gave us a buffer to use?
  buf = req->req_type.build_bpb_req.buffer_ptr;
  reply = fujicom_command_read(&cmd, buf, sizeof(sector_buf));
  if (reply != 'C') {
    consolef("FujiNet read fail: %i\n", reply);
    return ERROR_BIT;
  }

  //dumpHex(buf[128*2], 128);

#if 0
  consolef("\n");
  dumpHex((uint8_t far *) fn_bpb_pointers[req->unit], sizeof(struct BPB_struct));
  dumpHex(&buf[0x0b], sizeof(struct BPB_struct));
#endif

  _fmemcpy(fn_bpb_pointers[req->unit], &buf[0x0b], sizeof(struct BPB_struct));

#ifdef DEBUG
  dumpHex((uint8_t far *) fn_bpb_pointers[req->unit], sizeof(struct BPB_struct));
#endif // DEBUG
  
  req->req_type.build_bpb_req.BPB_table = MK_FP(getCS(), fn_bpb_pointers[0]);

  return OP_COMPLETE;
}

uint16_t Ioctl_input_cmd(SYSREQ far *req)
{
  return UNKNOWN_CMD;
}

uint16_t Input_cmd(SYSREQ far *req)
{
  char reply;
  uint16_t idx, sector = req->req_type.i_o_req.start_sector;
  uint8_t far *buf = req->req_type.i_o_req.buffer_ptr;


#if 0
  dumpHex((uint8_t far *) req, req->length);
  consolef("SECTOR: 0x%x\n", sector);
#endif
  
  for (idx = 0; idx < req->req_type.i_o_req.count; idx++, sector++) {
    cmd.device = DEVICEID_DISK;  /* Drive 1 */
    cmd.comnd = 'R'; /* Read */
    cmd.aux1 = sector & 0xFF;
    cmd.aux2 = sector >> 8;

    reply = fujicom_command_read(&cmd, &buf[idx * SECTOR_SIZE], SECTOR_SIZE);
    if (reply != 'C')
      break;
  }
  if (!idx)
    return ERROR_BIT;
  
  req->req_type.i_o_req.count = idx;
  return OP_COMPLETE;
}

uint16_t Input_no_wait_cmd(SYSREQ far *req)
{
  return UNKNOWN_CMD;
}

uint16_t Input_status_cmd(SYSREQ far *req)
{
  return UNKNOWN_CMD;
}

uint16_t Input_flush_cmd(SYSREQ far *req)
{
  return OP_COMPLETE;
}

uint16_t Output_cmd(SYSREQ far *req)
{
  req->req_type.i_o_req.count = 0;
  return OP_COMPLETE;
}

uint16_t Output_verify_cmd(SYSREQ far *req)
{
  req->req_type.i_o_req.count = 0;
  return OP_COMPLETE;
}

uint16_t Output_status_cmd(SYSREQ far *req)
{
  return UNKNOWN_CMD;
}

uint16_t Output_flush_cmd(SYSREQ far *req)
{
  return UNKNOWN_CMD;
}

uint16_t Ioctl_output_cmd(SYSREQ far *req)
{
  return UNKNOWN_CMD;
}

uint16_t Dev_open_cmd(SYSREQ far *req)
{
  return OP_COMPLETE;
}

uint16_t Dev_close_cmd(SYSREQ far *req)
{
  return UNKNOWN_CMD;
}

uint16_t Remove_media_cmd(SYSREQ far *req)
{
  return UNKNOWN_CMD;
}

uint16_t Ioctl_cmd(SYSREQ far *req)
{
  return UNKNOWN_CMD;
}

uint16_t Get_l_d_map_cmd(SYSREQ far *req)
{
  req->req_type.l_d_map_req.unit_code = 0;
  return OP_COMPLETE;
}

uint16_t Set_l_d_map_cmd(SYSREQ far *req)
{
  req->req_type.l_d_map_req.unit_code = 0;
  return OP_COMPLETE;
}

uint16_t Unknown_cmd(SYSREQ far *req)
{
  return UNKNOWN_CMD;
}
