
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:%.c=%.o)
LIB_OBJS := $(filter-out src/cli.o,$(OBJS))

BIN_NAME := ggjson
STATICLIB_NAME := libggjson.a
SHAREDLIB_NAME := libggjson.so

CFLAGS = -std=c17 -g -Iinclude/ -Ideps/nanbox_t/include

PREFIX := /usr/local

all: static shared
.PHONY: all clean install

deps: deps/nanbox_t

deps/nanbox_t:
	git clone --depth 1 https://github.com/fowlmouth/nanbox_t.git deps/nanbox_t
	cd deps/nanbox_t && $(MAKE) clean && $(MAKE)

src/%.o: src/%.c deps
	cc -c $(CFLAGS) -o $@ $< 

static: $(LIB_OBJS)
	ar rcs $(STATICLIB_NAME) $(LIB_OBJS) deps/nanbox_t/libnanbox.a

shared: $(LIB_OBJS)
	cc -shared -o $(SHAREDLIB_NAME) $(LIB_OBJS) deps/nanbox_t/libnanbox.a

clean:
	rm -rf $(OBJS) $(BIN_NAME) $(SHAREDLIB_NAME) $(STATICLIB_NAME)

install:
	install libggjson.so libggjson.a $(PREFIX)/lib
