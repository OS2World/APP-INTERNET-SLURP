#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>

#define BUFSIZE 1024
#define E_newsCache 128

#define checkref(a) { if (!a) exit(99); }

#define currentfile()
#define printerr( x )  printf("%s", x)

void printmsg(int level, char *fmt, ...);
