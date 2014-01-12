#
# Makefile for slurp
#

CC=icc
CFLAGS= -Ti
LDFLAGS= -Ti
LIBS= tcp32dll.lib so32dll.lib

SOURCE = slurp.c newnews.c articles.c history.c time.c sockets.c misc.c \
	 space.c hdbm.c idx.c syslog.c getopt.c getopt1.c fix_env.c pipe.c \
         printmsg.c cache.c

OBJECT = slurp.obj newnews.obj articles.obj history.obj time.obj \
         sockets.obj misc.obj space.obj hdbm.obj idx.obj syslog.obj \
         getopt.obj getopt1.obj fix_env.obj pipe.obj printmsg.obj \
         cache.obj

slurp.exe: $(OBJECT)
	$(CC) $(LDFLAGS) $(OBJECT) $(LIBS)

slurp.obj: slurp.c slurp.h conf.h nntp.h
newnews.obj: newnews.c slurp.h conf.h nntp.h
articles.obj: articles.c slurp.h conf.h nntp.h
history.obj: history.c slurp.h conf.h nntp.h
time.obj: time.c slurp.h conf.h
sockets.obj: sockets.c slurp.h conf.h nntp.h
misc.obj: misc.c slurp.h conf.h
space.obj: space.c slurp.h conf.h
syslog.obj: syslog.c syslog.h
getopt.obj: getopt.c getopt.h
getopt1.obj: getopt1.c getopt.h
fix_env.obj: fix_env.c fix_env.h
pipe.obj: pipe.c pipe.h

hdbm.obj: hdbm.c hdbm.h idx.h uupcmoah.h
idx.obj: idx.c idx.h uupcmoah.h
cache.obj: cache.c cache.h uupcmoah.h
printmsg.obj: uupcmoah.h

clean:
	-del *.obj slurp.exe

# END-OF-FILE
