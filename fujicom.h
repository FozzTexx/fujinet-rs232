/**
 * low level FujiNet API
 */  
  
#ifndef _FUJICOM_H
#define _FUJICOM_H
  
#include <stdint.h>
typedef struct /* Command Frame */ 
{
  uint8_t ddev;                /* Destination Device */
  uint8_t dcomnd;              /* Command */
  uint8_t daux1;               /* Auxiliary Parameter 1 */
  uint8_t daux2;               /* Auxiliary Parameter 2 */
  uint8_t dcksum;              /* 8-bit checksum */
} cmdFrame_t;


 
/**
 * @brief start fujicom
 */ 
void fujicom_init(void);


 
/**
 * @brief calculate 8-bit checksum for cmdFrame_t.dcksum
 * @param buf Buffer to compute checksum against
 * @param len Length of aforementioned buffer
 */ 
  uint8_t fujicom_cksum(uint8_t * buf, uint16_t len);
 
/**
 * @brief send FujiNet frame with no payload
 * @param cmdFrame Pointer to command frame
 * @return 'C'omplete, 'E'rror, or 'N'ak
 */ 
char fujicom_command(cmdFrame_t * c);


 
/**
 * @brief send fujinet frame and read payload
 * @param cmdFrame pointer to command frame
 * @param buf Pointer to buffer to receive
 * @param len Expected buffer length
 */ 
char fujicom_command_read(cmdFrame_t * c, uint8_t * buf, uint16_t len);


 
/**
 * @brief send fujinet frame and write payload
 * @param cmdFrame pointer to command frame
 * @param buf pointer to buffer to send.
 * @param len Length of buffer to send.
 */ 
char fujicom_command_write(cmdFrame_t * c, uint8_t * buf, uint16_t len);


 
/** 
 * @brief end fujicom
 */ 
void fujicom_done(void);


 
#endif  /* _FUJICOM_H */
