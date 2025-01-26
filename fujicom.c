/**
 * #FUJINET Low Level Routines
 */

#include "fujicom.h"
#include "com.h"

#include <env.h>
#include <stdlib.h>

#define IRQ_COM1	12
#define IRQ_COM2	11
#define BASE_COM1	0x3f8
#define BASE_COM2	0x2f8

#define TIMEOUT		100

PORT fn_port;
PORT *port;

void fujicom_init(void)
{
  int base, irq;
	int baud=9600;
	int p=1;

	if (getenv("FUJI_PORT"))
		p=atoi(getenv("FUJI_PORT"));

	if (getenv("FUJI_BAUD"))
		baud=atoi(getenv("FUJI_BAUD"));

	switch(p)
	{
		default:
		case 1:
			base = BASE_COM1;
			irq = IRQ_COM1;
			break;
		case 2:
			base = BASE_COM2;
			irq = IRQ_COM2;
			break;
	}

	port = port_open_static(&fn_port, base, irq);
	port_set(port,baud,'N',8,1);
}

unsigned char fujicom_cksum(unsigned char *buf, unsigned short len)
{
	unsigned int chk = 0;
	int i=0;

	for (i=0;i<len;i++)
		chk = ((chk+buf[i]) >> 8) + ((chk + buf[i]) & 0xFF);

	return (unsigned char)chk;
}

/**
 * @brief Internal function, send command, get response.
 *
 * @param c ptr to command frame to send
 * @return 'A'ck, or 'N'ak.
 */
char _fujicom_send_command(cmdFrame_t *c)
{
	int i=-1;
	unsigned char *cc = (unsigned char *)c;

	/* Calculate checksum and place in frame */
	c->dcksum = fujicom_cksum(cc,4);

	/* Assert DTR to indicate start of command frame */
	port_set_dtr(port,1);

	/* Write command frame */
	port_put(port,cc,sizeof(cmdFrame_t));

	/* Desert DTR to indicate end of command frame */
	port_set_dtr(port,0);

	i=port_getc_sync(port, TIMEOUT);

	return (unsigned char)i;
}

char fujicom_command(cmdFrame_t *c)
{
	_fujicom_send_command(c);

	return port_getc_sync(port, TIMEOUT);
}

char fujicom_command_read(cmdFrame_t *c, unsigned char *buf, unsigned short len)
{
	int r; /* response */
	int i;

	r = _fujicom_send_command(c);

	if (r == 'N')
		return r; /* Return NAK */

	/* Get COMPLETE/ERROR */

	r = port_getc_sync(port, TIMEOUT);

	if (r == 'C')
	{
		/* Complete, get payload */

		for (i=0;i<len;i++)
		{
			buf[i]=port_getc_sync(port, TIMEOUT);
		}

		/* Get Checksum byte, we don't use it. */
		port_getc_sync(port, TIMEOUT);
	}

	/* Otherwise, we got an error, return it. */
	return (unsigned char)r;
}

char fujicom_command_write(cmdFrame_t *c, unsigned char *buf, unsigned short len)
{
	unsigned char r; /* response */
	int i;
	unsigned char ck;

	r = _fujicom_send_command(c);

	if (r == 'N')
		return r;

	/* Write the payload */
	port_put(port,buf,len);

	/* Write the checksum */
	ck=fujicom_cksum(buf,len);
	port_putc(ck, port);

	/* Wait for ACK/NACK */
	r = port_getc_sync(port, TIMEOUT);
	
	if (r == 'N')
		return r;

	/* Wait for COMPLETE/ERROR */
	return port_getc_sync(port, TIMEOUT); 
}

void fujicom_done(void)
{
	port_close_static(port);
}
