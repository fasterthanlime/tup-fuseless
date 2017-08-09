CFLAGS = -Os -W -Wall -Wbad-function-cast -Wcast-align -Wcast-qual -Wchar-subscripts -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -Wswitch-enum -fno-common -I../../src -w -fPIC
LDFLAGS = -shared -Wl,-soname,tup-ldpreload.so -ldl

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

LTC = ../../libtup_client.a

all: tup-ldpreload.so

tup-ldpreload.so: $(OBJS) $(LTC)
	gcc $(OBJS) $(LTC) $(LDFLAGS) -o tup-ldpreload.so

%.o: %.c
	gcc -c $< -o $@ $(CFLAGS)


