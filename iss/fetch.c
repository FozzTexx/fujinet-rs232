/**
 * Fetch routine
 */

#include "fujicom.h"
#include <string.h>
#include <stdlib.h>

const char url[256]="N:HTTP://api.open-notify.org/iss-now.json";
const char query_lon[256]="N:/iss_position/longitude";
const char query_lat[256]="N:/iss_position/latitude";
const char query_ts[256]="N:/timestamp";
fujiStatus status;

/* Needs to be here to prevent it becoming __far */
char ts_s[16];

void fetch(char *lat, char *lon, unsigned long *ts)
{

	_fmemset(ts_s,0,sizeof(ts_s));

#if 0
	/* open */
	c.ddev    = 0x71;
	c.dcomnd = 'O';
	c.daux1  = 0x0C;
	c.daux2  = 0x00;
	fujicom_command_write(&c,(unsigned char *)url,sizeof(url));
#else
	fujiF5_write(DEVICEID_FN_NETWORK, CMD_OPEN, 0x000C, 0, url, sizeof(url));
#endif

#if 0
	/* Set channel mode to JSON */
	c.dcomnd = 0xFC;
	c.daux1  = 0x00;
	c.daux2  = 0x01;
	fujicom_command(&c);
#else
	fujiF5_none(DEVICEID_FN_NETWORK, CMD_JSON, 0x0100, 0, NULL, 0);
#endif

	/* Parse incoming JSON */
#if 0
	c.dcomnd = 'P';
	fujicom_command(&c);
#else
	fujiF5_none(DEVICEID_FN_NETWORK, CMD_PARSE, 0, 0, NULL, 0);
#endif

	/* Set query to timestamp */
#if 0
	c.dcomnd = 'Q';
	fujicom_command_write(&c,(unsigned char *)query_ts,sizeof(query_ts));
#else
	fujiF5_write(DEVICEID_FN_NETWORK, CMD_QUERY, 0, 0, &query_ts, sizeof(query_ts));
#endif

	/* Get # of bytes waiting for timestamp */
#if 0
	c.dcomnd = 'S';
	c.daux1=0;
	c.daux2=0;
	fujicom_command_read(&c,(unsigned char *)&s,sizeof(s));
#else
	fujiF5_read(DEVICEID_FN_NETWORK, CMD_STATUS, 0, 0, &status, sizeof(status));
#endif

	/* Read Timestamp */
#if 0
	c.dcomnd = 'R';
	c.daux1=s.bw;
	fujicom_command_read(&c,ts_s,s.bw);
#else
	fujiF5_read(DEVICEID_FN_NETWORK, CMD_STATUS, status.bw, 0, ts_s, status.bw);
#endif
	
	*ts=atol(ts_s);

	/* Set query for longitude */
#if 0
	c.dcomnd = 'Q';
	fujicom_command_write(&c,(unsigned char *)query_lon,sizeof(query_lon));
#else
	fujiF5_write(DEVICEID_FN_NETWORK, CMD_QUERY, 0, 0, query_lon, sizeof(query_lon));
#endif

	/* Get # of bytes waiting for timestamp */
#if 0
	c.dcomnd = 'S';
	c.daux1=0;
	c.daux2=0;
	fujicom_command_read(&c,(unsigned char *)&s,sizeof(s));
#else
	fujiF5_read(DEVICEID_FN_NETWORK, CMD_STATUS, 0, 0, ts_s, status.bw);
#endif

	/* Read Timestamp */
#if 0
	c.dcomnd = 'R';
	c.daux1=s.bw;
	fujicom_command_read(&c,lon,s.bw);
#else
	fujiF5_read(DEVICEID_FN_NETWORK, CMD_STATUS, status.bw, 0, ts_s, status.bw);
#endif

	/* Set query for latitude */
#if 0
	c.dcomnd = 'Q';
	fujicom_command_write(&c,(unsigned char *)query_lat,sizeof(query_lat));
#else
	fujiF5_write(DEVICEID_FN_NETWORK, CMD_QUERY, 0, 0, query_lat, sizeof(query_lat));
#endif

	/* Get # of bytes waiting for timestamp */
#if 0
	c.dcomnd = 'S';
	c.daux1=0;
	c.daux2=0;
	fujicom_command_read(&c,(unsigned char *)&s,sizeof(s));
#else
	fujiF5_read(DEVICEID_FN_NETWORK, CMD_STATUS, 0, 0, ts_s, status.bw);
#endif

	/* Read Timestamp */
#if 0
	c.dcomnd = 'R';
	c.daux1=s.bw;
	fujicom_command_read(&c,lat,s.bw);
#else
	fujiF5_read(DEVICEID_FN_NETWORK, CMD_STATUS, status.bw, 0, lat, status.bw);
#endif
}
