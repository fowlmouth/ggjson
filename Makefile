
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:%.c=%.o)
LIB_OBJS := $(filter-out src/cli.o,$(OBJS))
TEST_SRCS := $(wildcard test/*.c)
TEST_OBJS := $(TEST_SRCS:%.c=%.o)

BIN_NAME := ggjson
STATICLIB_NAME := libggjson.a
SHAREDLIB_NAME := libggjson.so

STD := -std=c17
CFLAGS := $(STD) -g -Iinclude/ -Ideps/nanbox_t/include
TEST_CFLAGS := $(CFLAGS) -Ideps/libtest/include

PREFIX := /usr/local

all: static shared
.PHONY: all clean cleandeps install

deps: deps/nanbox_t deps/libtest

deps/nanbox_t:
	git clone --depth 1 https://github.com/fowlmouth/nanbox_t.git deps/nanbox_t
	cd deps/nanbox_t && $(MAKE) clean && $(MAKE)

deps/libtest:
	git clone --depth 1 https://github.com/fowlmouth/libtest.git deps/libtest

src/%.o: src/%.c deps
	cc -c -o $@ $< $(CFLAGS)

$(STATICLIB_NAME): $(LIB_OBJS)
	ar rcs $(STATICLIB_NAME) $(LIB_OBJS) deps/nanbox_t/libnanbox.a

$(SHAREDLIB_NAME): $(LIB_OBJS)
	cc -shared -o $(SHAREDLIB_NAME) $(LIB_OBJS) deps/nanbox_t/libnanbox.a

static: $(STATICLIB_NAME)
shared: $(SHAREDLIB_NAME)

test/%.o: test/%.c deps
	cc -c -o $@ $< $(TEST_CFLAGS)

tests: $(TEST_OBJS) $(STATICLIB_NAME)
	cc -o $@ $(TEST_OBJS) $(TEST_CFLAGS) $(STATICLIB_NAME) deps/libtest/libtest.c

clean:
	rm -rf $(OBJS) $(BIN_NAME) $(SHAREDLIB_NAME) $(STATICLIB_NAME)
	rm -rf $(TEST_OBJS) tests

cleandeps:
	rm -rf deps

cleanall: clean cleandeps

install:
	install libggjson.so libggjson.a $(PREFIX)/lib
