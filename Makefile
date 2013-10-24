-include config.mk

OBJS += src/main.o

CFLAGS += -g -Wall -Ideps/libuv/include -Iinclude

uname_S=$(shell uname -s)

ifeq (Darwin, $(uname_S))
	LDFLAGS += -framework CoreServices
endif

all: libuv main

main: $(OBJS)
	gcc $^ deps/libuv/libuv.a $(LDFLAGS) -o $@

libuv:
	$(MAKE) -C deps/libuv/

.PHONY: all
