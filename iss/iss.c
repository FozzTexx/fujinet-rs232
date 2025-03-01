/**
 * @brief   ISS tracker for MS-DOS systems
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license GPL v. 3, see LICENSE for details.
 */

#include <conio.h>
#include <time.h>
#include <dos.h>
#include "grlib.h"
#include "map.h"
#include "sat.h"
#include "fetch.h"
#include "osd.h"

char lat[16], lon[16];
static unsigned long ts;
int timeout=60000;

void main()
{
	int oldmode = gr_mode(-1);

	gr_mode(4);
	gr_color(0,1);

	while(1)
	{
		map();
		fetch(&lat,&lon,&ts);
		osd(&lat,&lon,ts);
		sat((char *)&lat,(char *)&lon);
		while (timeout--)
		{
			delay(1);
			if (kbhit())
			{
				switch (getch())
				{
					case 0x1B:
						goto bye;
					default:
						timeout=1;
						break;
				}
			}
		}
	}

bye:
	gr_mode(oldmode);

}
