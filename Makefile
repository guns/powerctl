PREFIX ?= /usr/local/bin
CC = clang
CFLAGS ?= -Weverything -static

all:
	$(CC) $(CFLAGS) powerctl.c -o powerctl

clean:
	rm -f powerctl

install: all
	install -d $(DESTDIR)$(PREFIX)
	install -m 0755 powerctl $(DESTDIR)$(PREFIX)
