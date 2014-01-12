#include "conf.h"
#include "nntp.h"


/* Standard header files */

#include <sys/types.h>
#include "syslog.h"
#include <stdio.h>
#include <io.h>
#include <direct.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>

#include <string.h>
#include <time.h>

extern char *hostname;		/* Name of current NNTP server host */
extern char *pname;			/* Name of this program */
extern int  debug_flag;		/* Write extra debugging output to screen */
extern int  rnews;
extern int  no_time_flag;	/* Don't update slurp.<hostname> */
extern int  no_id_load_flag;    /* Don't dump / load message ids */
extern int  speedup_flag;       /* Do overlapping NNTP commands */
extern int  verbose_flag;       /* Print progress */

extern char sysfile[];
extern char timfile[];
extern char indir[];
extern char *historyfile;

/* Article counters */

extern int  dupart;			/* Number of duplicate articles */
extern int  misart;			/* Number of missing articles */
extern int  newart;			/* Number of new articles */

extern long totalsize;		/* Total size of articles tranferred */

/* Details for NEWNEWS */

extern char *nn_newsgroups;
extern char *nn_time;
extern char *nn_distributions;

/* Binary tree holding message ids */

struct mnode
	{
	struct mnode *left;
	struct mnode *right;
	char *msgid;
	int used;
	};
                          
extern struct mnode *root;
extern int entries;


/* Slurp function prototypes */

extern void get_articles ();						/* articles.c */
extern void enqueue_batch ();

extern int  open_history ();						/* history.c */
extern void close_history ();
extern int  check_id (char *message_id);

extern void log_ret (const char *fmt, ...);			/* misc.c */
extern void log_sys (const char *fmt, ...);
extern void log_msg (const char *fmt, ...);
extern char *stradd (const char *, const char *);

extern void get_ids ();								/* newnews.c */
extern void process_id (char *msgid);

extern void set_ntime ();							/* slurp.c */

extern int  tcp_open (char *host, char *service);	/* sockets.c */
extern int  server_init (char *hostname);
extern void close_server ();
extern void get_server (char *buf, int size);
extern void put_server (char *buf);

extern int  space (int min_free);					/* space.c */

extern time_t server_time ();						/* time.c */

/* The inevitable... */

#define TRUE (1)

#define FALSE (0)

#define PATH_MAX 1024

/* END-OF-FILE */
