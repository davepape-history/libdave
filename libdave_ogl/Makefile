INCLUDES = -I.
#OBJS = 	libdave.o libdave_iris.o dump_sgi.o dump_shrunk.o color.o texmaps.o
OBJS = 	libdave.o color.o libdave_iris.o
CFLAGS = -O2 -n32

libdave_ogl.a: $(OBJS)
	ar scr libdave_ogl.a $?

.c.o:
	cc -c $(CFLAGS) $(INCLUDES) $<
