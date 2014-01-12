/*
 * history - handle a news history file
 *
 * Copyright (C) 1992/93 Stephen Hebditch. All rights reserved.
 * TQM Communications, BCM Box 225, London, WC1N 3XX.
 * steveh@orbital.demon.co.uk  +44 836 825962
 *
 * See README for more information and disclaimers
 *
 * Routines to open and close a C-News style history file and determine
 * whether or not a particular message id exists in the history file.
 *
 * $Id: history.c,v 1.7 1993/06/07 11:07:14 root Exp $
 *
 * $Log: history.c,v $
 * Revision 1.7  1993/06/07  11:07:14  root
 * If neither DBZ, DBM or NDBM are defined then don't carry out
 * any history file lookups.
 *
 * Revision 1.6  1993/04/22  18:07:11  root
 * No changes - put back in RCS after the RCS file went missing...
 *
 * Revision 1.4  1993/02/14  14:51:59  root
 * No changes.
 *
 * Revision 1.0  1992/09/92
 * Initial coding.
 *
 */

#include "slurp.h"

#include "hdbm.h"
#include <fcntl.h>
static DBM *db = NULL;


/*
 * open_history - Open history file
 */

	int
open_history ()
	{
	if (historyfile)
	 	if ((db = dbm_open (historyfile, O_RDONLY, 0)) == NULL)
			return (1);
	return (0);
	}


/*
 * close_history - Close history file
 */

	void
close_history ()
	{
	if (historyfile)
	 	dbm_close (db);
	}


/*
 * Determine if message id already exists in the history file
 */

	int
check_id (char *message_id)
	{
	datum k, d;

/* Now check for presence with dbm/ndbm */

	if (historyfile) {

		k.dptr = message_id;
		k.dsize = strlen (message_id) + 1;

	 	d = dbm_fetch (db, k);
		return (d.dptr == NULL);
		}
	return (1);
	}

/* END-OF-FILE */
