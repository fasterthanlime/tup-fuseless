
all:
	$(MAKE) -C src/tup
	$(MAKE) -C src/tup/tup
	$(MAKE) -C src/tup/monitor
	$(MAKE) -C src/tup/flock
	$(MAKE) -C src/tup/server
	$(MAKE) -C src/tup/version
	$(MAKE) -C src/inih
	$(MAKE) -C src/compat
	$(MAKE) -C src/lua
	$(MAKE) -C src/sqlite3
	ar crs libtup_client.a src/tup/vardict.o src/tup/send_event.o src/tup/flock/fcntl.o
	$(MAKE) -C src/ldpreload
	cp src/tup/vardict.h tup_client.h
	gcc \
		src/tup/*.o \
		src/tup/tup/*.o \
		src/tup/monitor/*.o \
		src/tup/flock/*.o \
		src/tup/server/*.o \
		src/tup/version/*.o \
		src/inih/*.o \
		src/compat/*.o \
		src/sqlite3/*.o \
		src/lua/*.a \
		-o tup $(shell pkg-config fuse --libs) -lm -lpthread -ldl

clean:
	rm $(shell find ./ -name '*.[oa]')