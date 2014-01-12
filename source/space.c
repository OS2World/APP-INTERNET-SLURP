/*
 * space - determine free space on a filesystem
 *
 * OS/2 version (c) Jochen Friedrich 1995
 *
 */

#define INCL_DOSFILEMGR
#include <os2.h>

#undef TRUE
#undef FALSE

#include "slurp.h"

#define DFREE_OK     0
#define DFREE_USAGE  1
#define DFREE_ERR    2
#define DFREE_BLOCKS 3


/*
 * dfree - Check for free space available on the file system containing
 * the specified directory. Space is measured in kilobytes.
 */

	static int
dfree (char *location, int free_space)
	{
	FSALLOCATE fsa;

	if (strchr(location,':') != &(location[1]))
		return (DFREE_USAGE);	/* no absolute path */

	if (DosQueryFSInfo(toupper(location[0])-'A'+1,FSIL_ALLOC,&fsa,
			   sizeof(fsa)))
		return (DFREE_ERR);	/* error during query */

	/* Test if blocks are available */

	if (fsa.cUnitAvail * fsa.cSectorUnit * fsa.cbSector < free_space*1024)
		return (DFREE_BLOCKS);

	return (DFREE_OK);
	}


/*
 * space - Returns 1 if there are a sufficient number of free blocks
 * and inodes on the filesystem containing the news spool, or 0 if
 * there are only a small number of blocks / inodes remaining.
 */

	int
space (int min_free)
	{
	char *uupcsysrc;
	char buffer[1024];
	char spooldir[_MAX_DIR];
	FILE *rcfile;

	spooldir[0] = '\0';

	if (rnews) {
		uupcsysrc = getenv("UUPCSYSRC");
		if (!uupcsysrc) {
			log_msg ("space: undefined environment: UUPCSYSRC");
			return (0);
		}
		rcfile = fopen(uupcsysrc,"r");
		while (fgets(buffer,sizeof(buffer),rcfile)) {
			if (!strnicmp(buffer,"SPOOLDIR=",9))
				strcpy(spooldir,buffer+9);
		}
		fclose(rcfile);
		if (!strcmp(spooldir,"")) {
			log_msg ("space: can't find spool directory");
			return (0);
		}
	} else {
		if (getcwd(spooldir,_MAX_DIR) == 0) {
			log_msg ("space: can't query current directory");
			return (0);
		}
	}
	switch (dfree (spooldir, min_free))
		{
		case DFREE_OK:
			return (1);
		case DFREE_USAGE:
			log_msg ("space: no full path: %s", spooldir);
			break;
		case DFREE_ERR:
			log_ret ("space: dfree failed due to system call error");
			break;
		case DFREE_BLOCKS:
			log_msg ("space: no space on %s", spooldir);
			break;
		}
	return (0);
	}

/* END-OF-FILE */
