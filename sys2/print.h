#ifndef _PRINT_H
#define _PRINT_H

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

extern void printHex(unsigned int val, unsigned int width, char leading);

// FIXME - get rid of these
extern void byte_to_hex(char *buffer, unsigned char byte);
extern void byte_to_decimal(char *buffer, unsigned char byte);

#endif /* _PRINT_H */
