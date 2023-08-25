all: bigint_test

CC=gcc
CLIBS=
CFLAGS=-g -Wall -pedantic -std=c99 -MMD
LDFLAGS=-g -Wall -pedantic -std=c99

bigint_test: bigint_test.o bigint_basic.o bigint_read.o bigint_write.o bigint_manip.o bigint_ops.o

clean:
	-rm -rf *.o *.d bigint_test
tidy: clean
	-rm -rf *~

-include *.d

.PHONY: all clean tidy

