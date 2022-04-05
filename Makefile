
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:%.c=%.o)
LIB_OBJS := $(filter-out src/cli.o,$(OBJS))
TEST_SRCS := $(wildcard test/*.c)
TEST_OBJS := $(filter-out test/ast.o,$(TEST_SRCS:%.c=%.o))

BIN_NAME := ggjson
STATICLIB_NAME := libggjson.a
SHAREDLIB_NAME := libggjson.so

STD := -std=c17
CFLAGS := $(STD) -Iinclude/ -Ideps/nanbox_t/include

LDFLAGS := deps/nanbox_t/libnanbox.a

PREFIX := /usr/local

DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG -g -O0
else
	CFLAGS += -DNDEBUG
endif

TEST_CFLAGS := $(CFLAGS) -Ideps/libtest/include

all: static shared
.PHONY: all clean cleandeps install

deps: deps/nanbox_t deps/libtest

deps/nanbox_t:
	git clone --depth 1 https://github.com/fowlmouth/nanbox_t.git deps/nanbox_t
	cd deps/nanbox_t && $(MAKE) clean && $(MAKE)

deps/libtest:
	git clone --depth 1 https://github.com/fowlmouth/libtest.git deps/libtest

src/%.o: src/%.c deps
	cc $(CFLAGS) -c -o $@ $< -fPIC

$(STATICLIB_NAME): $(LIB_OBJS)
	ar rcs $(STATICLIB_NAME) $(LIB_OBJS)

$(SHAREDLIB_NAME): $(LIB_OBJS)
	cc $(CFLAGS) -shared -o $(SHAREDLIB_NAME) $(LIB_OBJS) $(LDFLAGS)

static: $(STATICLIB_NAME)
shared: $(SHAREDLIB_NAME)

test/%.o: test/%.c deps
	cc $(TEST_CFLAGS) -c -o $@ $<

tests: $(TEST_OBJS) $(LIB_OBJS)
	cc $(TEST_CFLAGS) -o $@ $^ deps/libtest/libtest.c $(LDFLAGS)

clean:
	rm -rf $(OBJS) $(BIN_NAME) $(SHAREDLIB_NAME) $(STATICLIB_NAME)
	rm -rf $(TEST_OBJS) tests

cleandeps:
	rm -rf deps

cleanall: clean cleandeps

install:
	install libggjson.so libggjson.a $(PREFIX)/lib
