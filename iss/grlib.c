/**
 * @brief quick and dirty graphics lib
 */

#include <dos.h>

/* Need to be here otherwise they become __far */
union REGS r1,r2;

int gr_mode(int mode)
{

	if (mode<0)
	{
		r1.h.ah = 0x0f;
	}
	else
	{
		r1.h.ah = 0x00;
		r1.h.al = mode;
	}

	int86(0x10,&r1,&r2);

	return r2.h.al;
}

int gr_color(char p, char c)
{

	r1.h.ah = 0x0b;
	r1.h.bh = 0x01;
	r1.h.bl = p;

	/* int86(0x10,&r,0); */

	r1.h.ah = 0x0b;
	r1.h.bh = 0x00;
	r1.h.bl = c;

	int86(0x10,&r1,0);
	return r1.x.ax;
}

int gr_pset(int x, int y, char c)
{

	r1.h.ah = 0x0c;
	r1.h.al = c;
	r1.h.bh = 0;
	r1.x.cx = x;
	r1.x.dx = y;

	int86(0x10,&r1,0);
	return r1.x.ax;
}

int gr_text(int x, int y, char *s)
{

	/* Position cursor */
	r1.h.ah = 0x02;
	r1.h.bh = 0;
	r1.h.dh = y;
	r1.h.dl = x;
	int86(0x10,&r1,0);

	/* Output characters */
	while (*s)
	{
		r1.h.ah = 0x0e;
		r1.h.al = *s;
		r1.h.bl = 0x0f;

		int86(0x10,&r1,0);

		s++;
	}

	return r1.x.ax;
}
