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
extern void printDTerm(const char *);
#pragma aux printDTerm =      \
    "mov    ah, 0x9"          \
    "int    0x21"             \
    __parm [__dx]             \
    __modify [__ax __di __es];

extern void printHex(uint16_t val, uint16_t width, char leading);
extern void printHex32(uint32_t val, uint16_t width, char leading);
extern void printDec(uint16_t val, uint16_t width, char leading);
extern void dumpHex(uint8_t far *buffer, uint16_t count);

extern void consolef(const char *format, ...);

#endif /* _PRINT_H */
