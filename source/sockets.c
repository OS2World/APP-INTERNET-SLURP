/*
 * sockets - open a socket connection and read/write to nntp server
 *
 * Copyright (C) 1992/93 Stephen Hebditch. All rights reserved.
 * TQM Communications, BCM Box 225, London, WC1N 3XX.
 * steveh@orbital.demon.co.uk  +44 836 825962
 *
 * See README for more information and disclaimers
 *
 * Obtain the current time from the remote server in standard unix time
 * format for use with the next NEWNEWS. If the client is unable to
 * connect to the time server, then local time is used instead.
 *
 * $Id: sockets.c,v 1.7 1993/06/23 10:15:37 root Exp $
 *
 * $Log: sockets.c,v $
 * Revision 1.7  1993/06/23  10:15:37  root
 * Replaced bzero/bcopy definitions with ANSI memcpy and memset.
 * Duplicate fd before using fdopen separately on each fd for read
 * and write, fixing long-standing stdio memory problems.
 *
 * Revision 1.6  1993/04/22  18:25:16  root
 * If NOBUFFOUT defined then turn off stdio buffering for the
 * output stream to the server to get round problem with SVR3s.
 *
 * Revision 1.5  1993/03/01  18:00:18  root
 * Use ferror to detect erros, not return code.
 *
 * Revision 1.4  1993/02/14  16:22:42  root
 * No longer have get_server return a return code. Makes this module
 * no longer compatible with nntp 1.6 client library, but there ya go...
 * Changed error detection in put_server for no other reason than to
 * be consistent with elsewhere.
 *
 * Revision 1.3  1992/12/15
 * Removed unnecessary close() in close_server.
 * Syslog log level for connected message changed to LOG_INFO.
 *
 * Revision 1.1  1992/12/04
 * Print line before it is sent to server when debugging is on.
 *
 * Revision 1.0  1992/11/29
 * Adapted from nntpxfer-e code.
 * Incorporate code to set up a tcp connection, plus cleaned up the
 * existing code.
 *
 */

#include "slurp.h"

#include <signal.h>
#include <setjmp.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef INADDR_NONE
  #define INADDR_NONE 0xffffffff
#endif

struct sockaddr_in serv_addr;
struct servent serv_info;
struct hostent host_info;

static int server_socket;


/*
 * tcp_open - Open a tcp connection to 'host' for service 'service',
 * returning a file descriptor for the socket.
 */

	int
tcp_open (char *host, char *service)
	{
	int sockfd;
	int on = 1;
	unsigned long inaddr;
	struct servent *sp;
	struct hostent *hp;

	(void) memset (&serv_addr, 0, sizeof (serv_addr));
	serv_addr.sin_family = AF_INET;

	/* Get service information */
	if ((sp = getservbyname (service, "tcp")) == NULL)
		{
		log_ret ("tcp_open: Unknown service %s/tcp", service);
		return (-1);
		}
	serv_info = *sp;
	serv_addr.sin_port = sp->s_port;

	/* Try to convert host name as dotted decimal */
	if ((inaddr = inet_addr (host)) != INADDR_NONE)
		{
		(void) memcpy (&serv_addr.sin_addr, &inaddr, sizeof (inaddr));
		host_info.h_name = NULL;
		}
	/* If that failed, then look up the host name */
	else
		{
		if ((hp = gethostbyname (host)) == NULL)
			{
			log_ret ("tcp_open: Host name error: %s", host);
			return (-1);
			}
		host_info = *hp;
		(void) memcpy (&serv_addr.sin_addr, hp->h_addr, hp->h_length);
		}

	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
		{
		log_ret ("tcp_open: Can't create TCP socket");
		return (-1);
		}

	if (connect (sockfd, (struct sockaddr *) &serv_addr,
	    sizeof (serv_addr)) < 0)
		{
		log_ret ("tcp_open: Can't connect to server %s", host);
		(void) soclose (sockfd);
		return (-1);
		}

	if (setsockopt (sockfd, SOL_SOCKET, SO_KEEPALIVE, 
					(char *) &on, sizeof (on)) < 0)
		log_ret ("tcp_open: Can't set KEEPALIVE on socket");

	return (sockfd);
	}

/*
 * server_init - Open a connection to the NNTP server. Returns -1 if an
 * error occurs, otherwise the server's initial response code.
 */

	int
server_init (char *hostname)
	{
	char line [NNTP_STRLEN];

	/* First try and make the connection */

	if (sock_init() < 0)
		return (-1);
	if ((server_socket = tcp_open (hostname, "nntp")) < 0)
		return (-1);
        
	/* Inform everyone that we're there */
	if (!debug_flag)
		syslog(LOG_INFO, "Connected to nntp server at %s", hostname);
	else
		(void) fprintf (stderr, "Connected to nntp server at %s\n", hostname);

	/* Get the greeting herald */
	get_server (line, sizeof (line));
	if (debug_flag)
		(void) fprintf (stderr, "-> %s\n", line);

	/* Return the banner code */
	return (atoi (line));
	}


/*
 * close_server - Close down the NNTP server connection
 */

	void
close_server ()
	{
	char line [NNTP_STRLEN];

	if (debug_flag)
		(void) fprintf (stderr, "<- QUIT\n");
	put_server ("QUIT");
	get_server (line, sizeof (line));
	if (debug_flag)
		(void) fprintf (stderr, "-> %s\n", line);

	soclose (server_socket);
	}

/*
 * get_server - Read a line up to CRLF from the socket into a buffer.
 */

int memtok(char *buffer, int buflen, char tok)
{
	int pos=1;
	if (!buflen) return 0;
	while (buflen--) {
		if (*buffer++==tok) return pos;
		pos++;
	}
	return 0;
}

int sogets(char *buffer, int buflen)
{
	static char data[NNTP_STRLEN];
	static int fuellung=0;
	int got;
	int i;

	if (buflen>NNTP_STRLEN) buflen=NNTP_STRLEN;
	if (got=memtok(data,fuellung,'\012')) {
		memcpy(buffer,data,got);
		for (i=0;i<fuellung-got;i++) data[i]=data[got+i];
		fuellung -= got;
		buffer[got]=0;
		return 1;
	}
  
	while ((!(i=memtok(data,fuellung,'\012'))) && (buflen-fuellung>0)) {
		got=recv(server_socket,data+fuellung,NNTP_STRLEN-fuellung,0);
		if (got<=0)
			return 0;
		fuellung +=got;
	}
	got = i;
	if (i>buflen) got=buflen;
	if (i==0) got=buflen;
	memcpy(buffer,data,got);
	for (i=0;i<fuellung-got;i++) data[i]=data[got+i];
	fuellung -= got;
	buffer[got]=0;
	return 1;
}

	void
get_server (char *line, int size)
	{
	char *pos;

	/* Read line */
	if (!sogets (line, size))
		log_sys ("get_server: Read error on server socket");

	/* Kill the CRLF */
	if (pos = strchr (line, '\r'))
		*pos = '\0';
	if (pos = strchr (line, '\n'))
		*pos = '\0';
	}

/*
 * put_server - write a line from a linefer to a socket
 */

	void
put_server (char *line)
	{
	char hline [NNTP_STRLEN];
	int length, ll;

	sprintf(hline, "%s\r\n", line);
	do {
		length=strlen(hline);
    		ll=send(server_socket,hline,length,0);
		if (ll==0) {
			log_sys ("put_server: Write error on server socket");
			return;
		}
		if(length-ll)
			memcpy(hline,hline+ll,length-ll);
		length-=ll;
	} while (length);
}

/* END-OF-FILE */
