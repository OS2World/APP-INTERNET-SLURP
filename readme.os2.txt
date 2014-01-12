This is the OS/2 port of slurp Version 1.08. Please see the file 'README' for more
informations on this software. This file describes differences between the OS/2
version and the original Unix version. Please report any bugs to jochen@audio.pfalz.de.

Changes for the OS/2 version:
=============================

For the OS/2 version, i did the following changes to the original code:

- all path statements are now adapted at run time. No modification of conf.h should be
  necessary.

- all file names have 8.3 character names. Slurp shold work on HPFS drives as well as
  on FAT drives.

- Slurp is compiled with hdbm which comes with UUPC/Extended. In order to use the
  unmodified original hdbm sources, a "fake" uupcmoah.h is included to generate a mini
  uupc environment.

- Support of history lookups and switch between storing spool files and directly calling
  rnews as well as the speedup feature are now command line options rather than compile 
  time options:
  "-s" will enable the spped up tricks.
  "-f" will feed the news batches directly into rnews.
  "-k" will make slurp use long filenames for its time files.
  "-v" will print a process indicator.
  "-h historyfile" will make lookups in the UUPC history file for each message id returned
  by the newnews command. If this command line option is missing, no history lookups
  will be done.

- News batch files will be written into the current directory and not into a special spool 
  directory.

Have fun with this version of OS/2 slurp !

Jochen.
