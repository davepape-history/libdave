INCLUDES = -I.
OBJS = 	libdave.o libdave_iris.o dump_sgi.o dump_shrunk.o color.o texmaps.o \
	matrix.o
CFLAGS = -O2 -n32 -mips3

libdave.a: $(OBJS)
	rm -f libdave.a
	ar scr libdave.a $?

.c.o:
	cc -c $(CFLAGS) $(INCLUDES) $<
