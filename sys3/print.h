#ifndef _PRINT_H
#define _PRINT_H

#include <stdint.h>

/* Print a single character */
extern void printChar(char);


#pragma aux printChar =		\
  "mov ah, 0x2"			\
  "int 0x21"			\
  __parm [__dl]			\
  __modify [__ax __di __es];

/* Use INT 21h to print. Buffer MUST be terminated with '$' so BIOS
   knows when to stop */
extern void printMsg(const char *);


#pragma aux printMsg =        \
    "mov    ah, 0x9"          \
    "int    0x21"             \
    __parm [__dx]             \
    __modify [__ax __di __es];

extern void printHex(uint16_t val, uint16_t width, char leading);


// FIXME - get rid of these
extern void byte_to_hex(char *buffer, uint8_t byte);
extern void byte_to_decimal(char *buffer, uint8_t byte);

#endif /* _PRINT_H */
