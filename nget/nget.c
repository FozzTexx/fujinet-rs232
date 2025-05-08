/**
 * @brief   Retrieve file from network endpoint
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license GPL v. 3, see LICENSE for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fujicom.h"

/* buffers for commands like OPEN are expected to be 256 bytes */
unsigned char url[256];

/* 512 byte buffer used for copy. */
unsigned char buf[512];

/* Status structure */
struct _status
{
	unsigned short bw;	 /* # of bytes waiting 0-65535 	*/
	unsigned char connected; /* Are we connected? 		*/
	unsigned char error;	 /* error code	1-255		*/
} status;

/**
 * @brief main nget function
 * @param src string ptr to the N: source URL
 * @param dst string ptr to the destination filename
 * @return The exit error code 0 = success
 */
int nget(char *src, char *dst)
{
	FILE *fp = fopen(dst,"wb");
	int err  = 0;
	cmdFrame_t c;
	unsigned long total=0;
	char *username=NULL, *password=NULL;

	fujicom_init();

	if ((src[0] != 'N') && (src[1] != ':'))
	{
		printf("\nInvalid N: URL. Terminating.\n");
		return 1;
	}

	if (!fp)
	{
		printf("\nCould not open destination file. Terminating.\n");
		return 2;
	}

	username=getenv("FUJI_USER");
	password=getenv("FUJI_PASS");

	if (username)
	{
		/* Perform username command */
		c.device = 0x71;
		c.comnd = 0xFD;
		c.aux1 = c.aux2 = 0x00;
		memset(url,0,sizeof(url));
		strcpy(url,username);
		fujicom_command_write(&c,url,sizeof(url));
	}

	if (password)
	{
		/* Perform password command */
		c.device = 0x71;
		c.comnd = 0xFE;
		c.aux1 = c.aux2 = 0x00;
		memset(url,0,sizeof(url));
		strcpy(url,password);
		fujicom_command_write(&c,url,sizeof(url));
	}

	memset(url,0,sizeof(url));

	/* Perform OPEN command */
	c.device   = 0x71;
	c.comnd = 'O';
	c.aux1  = 0x04; /* READ ONLY */
	c.aux2  = 0x00; /* NO TRANSLATION */
	strcpy(url,src);
	fujicom_command_write(&c,url,sizeof(url));

	/* Perform initial status command */
	c.device   = 0x71;
	c.comnd = 'S';
	c.aux1  = 0x00;
	c.aux2  = 0x00;
	fujicom_command_read(&c,(unsigned char *)&status,sizeof(status));

        if (status.error == NETWORK_ERROR_END_OF_FILE)
          status.error = NETWORK_SUCCESS;

	if (status.error > 1)
	{
		printf("\nOPEN ERROR: %u\n",status.error);
		return status.error;
	}

#if 0
	if (!status.connected)
	{
		printf("\nOPEN ERROR: Host immediately disconnected.\n");
		return 0xFF;
	}
#endif

	while (status.error != 136)
	{
		int bw = (status.bw > 512 ? 512 : status.bw);
		char reply = 0;

		/* Do read */
		c.comnd = 'R';
		c.aux1  = bw & 0xFF;
		c.aux2  = bw >> 8;
		reply = fujicom_command_read(&c,buf,bw);

		if (reply != 'C')
		{
			printf("\nREAD ERROR AT %lu bytes.\n",total);
			return 144;
		}

		/* Do write */
		fwrite(buf,bw,1,fp);

		total += bw;

		printf("%10lu bytes transferred.\r",total);

		/* Do next status */
		c.device = 0x71;
		c.comnd = 'S';
		c.aux1 = 0x00;
		c.aux2 = 0x00;
		fujicom_command_read(&c,
					(unsigned char *)&status, 
					sizeof(status));
	}

	/* Perform CLOSE command */
	c.device   = 0x71;
	c.comnd = 'C';
	fujicom_command(&c);

	fclose(fp);
	
	fujicom_done();

	return err;
}

/**
 * @brief show usage if incorrect # of parameters.
 */
void usage()
{
	printf("\nNGET <N:url> <fn>\n");
}

/**
 * @brief Main program entry
 */
int main(int argc, char *argv[])
{
	if (argc<3)
	{
		usage();
		return 1;
	}

	return nget(argv[1],argv[2]);
}
