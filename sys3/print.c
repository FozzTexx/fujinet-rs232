#include "print.h"

void printHex(uint16_t val, uint16_t width, char leading)
{
  uint16_t digits, tval;
  const char hex_digits[] = "0123456789ABCDEF";


  for (tval = val, digits = 0; tval; tval >>= 4, digits++);

  for (; digits < width; width--)
    printChar(leading);

  while (digits) {
    digits--;
    printChar(hex_digits[(val >> 4 * digits) & 0xf]);
  }

  return;
}

void byte_to_hex(char *buffer, uint8_t byte)
{
  const char hex_digits[] = "0123456789ABCDEF";


  buffer[0] = hex_digits[(byte >> 4) & 0xF];    // High nibble
  buffer[1] = hex_digits[byte & 0xF];   // Low nibble
  return;
}

void byte_to_decimal(char *buffer, uint8_t byte)
{
  buffer[0] = '0' + (byte / 10) % 10;
  buffer[1] = '0' + byte % 10;
  return;
}
