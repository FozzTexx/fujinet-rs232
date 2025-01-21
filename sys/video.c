#include <dos.h>
#include <string.h>

short Active_page(void)
{
  unsigned char page;

  _asm {
    mov ah, 0x0F;      // Set AH to 0x0F (function to get video mode and active page)
    int 0x10;          // Call BIOS interrupt 0x10
    mov page, bh;      // Store the active page from BH into the variable
  }

  return page;
}

void Goto_XY(short col, short row)
{
  unsigned char active_page = Active_page(); // Get the active page
  unsigned char adjusted_col = (unsigned char)(col - 1); // Adjust column (0-based)
  unsigned char adjusted_row = (unsigned char)(row - 1); // Adjust row (0-based)

  _asm {
    mov bh, active_page;      // Move active page into BH
    mov dh, adjusted_row;     // Move row into DH
    mov dl, adjusted_col;     // Move column into DL
    mov ah, 0x02;             // Set AH to 0x02 (Set cursor position)
    int 0x10;                 // Call BIOS interrupt 0x10
  }
}

char Get_char(void)
{
  unsigned char character;
  unsigned char active_page = Active_page(); // Get the active page

  _asm {
    mov bh, active_page; // Set active page in BH
    mov ah, 0x08;        // Set function to "Read character and attribute at cursor"
    int 0x10;            // Call BIOS interrupt 0x10
    mov character, al;   // Store the character (AL) in character
  }

  return character;
}

unsigned short Get_key(unsigned char mode)
{
  unsigned short key_code;

  _asm {
    mov ah, mode;        // Load the mode into AH
    int 0x16;            // Call BIOS interrupt 0x16 (keyboard services)
    mov key_code, ax;    // Store the full AX register (key code) in key_code
  }

  return key_code;
}

char Get_attr(void)
{
  unsigned char attribute;
  unsigned char active_page = Active_page(); // Get the active page

  _asm {
    mov bh, active_page; // Set active page in BH
    mov ah, 0x08;        // Set function to "Read character and attribute at cursor"
    int 0x10;            // Call BIOS interrupt 0x10
    mov attribute, ah;   // Store the attribute (AH) in attribute
  }

  return attribute;
}

short Get_X(void)
{
  unsigned char col;
  unsigned char bh = Active_page(); // Get the active page

  _asm {
    mov bh, bh;      // Set active page in BH
    mov ah, 0x03;    // Set function to "Read cursor position"
    int 0x10;        // Call BIOS interrupt 0x10
    mov col, dl;     // Store the column (DL) in col
  }

  return col + 1; // Adjust to 1-based index
}

short Get_Y(void)
{
  unsigned char row;
  unsigned char bh = Active_page(); // Get the active page

  _asm {
    mov bh, bh;      // Set active page in BH
    mov ah, 0x03;    // Set function to "Read cursor position"
    int 0x10;        // Call BIOS interrupt 0x10
    mov row, dh;     // Store the row (DH) in row
  }

  return row + 1; // Adjust to 1-based index
}

unsigned char Get_mode(void)
{
  unsigned char mode;

  _asm {
    mov ah, 0x0F;    // Set function to "Get video mode"
    int 0x10;        // Call BIOS interrupt 0x10
    mov mode, al;    // Store the mode (AL) in mode
  }

  return mode;
}

void Set_mode(unsigned char mode)
{
  _asm {
    mov al, mode;      // Set the video mode in AL
    mov ah, 0x00;      // Function 0x00: Set video mode
    int 0x10;          // Call BIOS interrupt 0x10
  }
}

void Clear_screen(void)
{
  unsigned char mode = Get_mode(); // Save the current video mode

  _asm {
    mov bh, 0x00;         // Video page number (0)
    mov cx, 0x0000;       // Start at the top-left corner (row 0, column 0)
    mov dx, 0x184F;       // End at row 24, column 79
    mov ax, 0x0600;       // Function 0x06: Scroll window up (clear screen)
    int 0x10;             // Call BIOS interrupt 0x10
  }

  Set_mode(mode);          // Restore the original video mode
  Goto_XY(1, 1);           // Move the cursor to the top-left corner
}

void Write_chr(unsigned char chr)
{
  unsigned char attr = Get_attr(); // Get the current attribute

  _asm {
    mov bl, attr;        // Move the attribute to BL
    mov al, chr;         // Move the character to AL
    mov ah, 0x0E;        // Set AH to 0x0E (Write character to TTY)
    int 0x10;            // Call BIOS interrupt 0x10
  }
}

static unsigned short es_static;
static unsigned short bp_static;

void Write_tty(unsigned char *printme)
{
  unsigned char x, y, page, attr;
  unsigned short len;

  // Get initial values
  x = Get_X();
  y = Get_Y();
  len = strlen(printme);
  page = Active_page();
  attr = Get_attr();

  // Save current ES and BP registers
  _asm {
    mov es_static, es      // Save current ES
    mov bp_static, bp      // Save current BP
  }

  // Prepare registers and call the BIOS interrupt
  _asm {
    mov cx, len            // String length
    dec y                  // Adjust for 0-based row
    mov dh, y              // Set row in DH
    dec x                  // Adjust for 0-based column
    mov dl, x              // Set column in DL
    mov bh, page           // Active page in BH
    mov bl, attr           // Attribute in BL
    push ds
    pop es
    mov bp, word ptr [printme]          // Load address of string into BP
    mov ax, 0x1301         // AH=0x13 (write TTY string), AL=01h (write to page)
    int 0x10               // Call BIOS interrupt 0x10
  }

  // Restore original ES and BP registers
  _asm {
    mov es, es_static      // Restore ES
    mov bp, bp_static      // Restore BP
  }
}
