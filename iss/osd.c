/**
 * OSD
 */

#include <stdio.h>
#include <time.h> 
#include "grlib.h"
#include "ftime.h"

/* Need to be here otherwise becomes __far */
static Timestamp ts;
char tmp[41];

void osd(char *lat, char *lon, unsigned long t)
{
	timestamp(t,&ts);

	gr_text(10,21,"CURRENT ISS POSITION");

	sprintf(tmp,"        %3s %3s %02d %02d:%02d:%02d %04d",
		time_dow(ts.dow),
		time_month(ts.month),
		ts.day,
		ts.hour,
		ts.min,
		ts.sec,
		ts.year);

	gr_text(0,22,tmp);
	sprintf(tmp,"           LATITUDE: %10s",lat);
	gr_text(0,23,tmp);
	sprintf(tmp,"          LONGITUDE: %10s",lon);
	gr_text(0,24,tmp);
}
