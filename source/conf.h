/*
 * conf.h - compilation options for slurp
 *
 * Copyright (C) 1992/93 Stephen Hebditch. All rights reserved.
 * TQM Communications, BCM Box 225, London, WC1N 3XX.
 * steveh@orbital.demon.co.uk  +44 836 825962
 *
 * See README for more information and disclaimers
 *
 * $Id: conf.h,v 1.8 1993/08/20 10:36:02 root Exp root $
 *
 * $Log: conf.h,v $
 * Revision 1.8  1993/08/20  10:36:02  root
 * Removed SIGRET definition as unused.
 *
 * Revision 1.7  1993/06/23  10:22:38  root
 * Removed NOBUFFOUT option.
 * Amended description for DBM, NDBM and DBZ as can now have none
 * defined if don't want to do history lookups.
 * Amended RNEWS description to describe how to send batches via UUCP.
 * Upped MAXCACHE value and improved description.
 *
 * Revision 1.6  1993/04/22  18:28:01  root
 * Added new compilation options for NOBUFFOUT and UNISTD.
 *
 * Revision 1.5  1993/03/01  17:41:09  root
 * Changed USG definition to SYSV, now used solely in slurp.h.
 * Added explanation of defines for space.c.
 *
 * Revision 1.4  1993/02/14  14:36:16  root
 * Removed BATCHARTSMAX, MSGIDMAX.
 * Added INDIR, BATCHNAME.
 * Modified TIMFILE, RNEWS.
 *
 * Revision 1.0  1992/10/29
 * Initial coding.
 *
 */


/* SLURP CONFIGURATION
   =================== */

#define SYSFILE			"c:\\etc\\slurp.sys"
#define TIMFILE			"c:\\etc\\slurp"
#define MAXCACHE		8192
#define RNEWS			"rnews.cmd"
#define BATCHSIZEMAX		300000L
#define MINFREE 		4000
#define SYSLOG			LOG_NEWS
#define COPYSIZE		16384

/* END-OF-FILE */
