CFLAGS = -Os -W -Wall -Wbad-function-cast -Wcast-align -Wcast-qual -Wchar-subscripts -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -Wswitch-enum -fno-common -D_FILE_OFFSET_BITS=64 -I../../src -w -DLUA_USE_POSIX 

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

all: liblua.a

liblua.a: $(OBJS)
	ar crs liblua.a $(OBJS)
#	gcc -c lua.c -o lua.o $(CFLAGS)
#	gcc lua.o liblua.a -o lua -lm 

%.o: %.c
	gcc -c $< -o $@ $(CFLAGS)

