# Makefile for everybit
# Copyright (C) 2009, 2010, 2011, 2012 the Massachusetts Institute of Technology
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


# This Makefile is set up to build the everybit library, as well as the test
# harness, in one of two modes.  In debug mode, the C compiler does virtually
# no optimizations (-O0) and embeds a bunch of debugging symbols to make work
# with GDB easier.  In release mode, the compiler will do every optimization it
# can (-O3) and keep debugging symbols out; this should decrease the size of
# the final binary and make it perform better.
#
# To compile in debug mode, type "make DEBUG=1".  To to compile in release
# mode, type "make DEBUG=0" or simply "make".
#
# If everything gets wacky and you need a sane place to start from, you can
# type "make clean", which will remove all compiled code.
#
# This code is portable to compilers other than icc--indeed, it should compile
# under any compiler which implements the C99 standard.  You can specify a
# different compiler--e.g., GCC--by passing CC=whatever on the command line.
# If you use a compiler whose option syntax is not GCC-compatible (e.g.,
# clang), you may need to specify CFLAGS and LDFLAGS explicitly as well.
#
# If you want to do something wacky with your compiler flags--like enabling
# debug symbols but keeping optimizations on--you can specify CFLAGS or LDFLAGS
# on the command line.  If you want to use a predefined mode but augment the
# predefined CFLAGS or LDFLAGS, you can specify EXTRA_CFLAGS or EXTRA_LDFLAGS
# on the command line.


# The sources we're building
SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)

# What we're building
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
PRODUCT = everybit

# What we're building with
CC = clang
CFLAGS = -std=c99 -Wall -m64 -g
LDFLAGS = -flto -fuse-ld=gold 

# We need to link against the timing library for whatever OS we're on.
PLATFORM = $(shell uname)
ifeq ($(PLATFORM),Linux)
	LDFLAGS += -lrt
else ifeq ($(PLATFORM),Darwin)
	LDFLAGS += -arch x86_64 -framework CoreServices
endif


# Determine which profile--debug or release--we should build against, and set
# CFLAGS appropriately.
#
# At this time, we also update the .buildmode stamp, which records if the last
# build was in debug or release mode.  This is a bit hackish--we set all C
# compiler outputs to depend on .buildmode, and then we touch .buildmode if it
# should change.  Touching .buildmode invalidates all the compiler outputs, so
# they all get built again in the correct mode.  Credit to Ceryen Tan and Marek
# Olszewski, who developed this code for 6.197 back in the day.
OLD_MODE = $(shell cat .buildmode 2> /dev/null)
ifeq ($(DEBUG),1)
# We want debug mode.
CFLAGS += -O0
ifneq ($(OLD_MODE),debug)
$(shell echo debug >.buildmode)
endif
else
# We want release mode.
CFLAGS += -O3 -DNDEBUG
ifneq ($(OLD_MODE),release)
$(shell echo release >.buildmode)
endif
endif


# By default, make the product.
all:		$(PRODUCT)

# How to compile a C file
%.o:		%.c $(HEADERS) .buildmode
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ -c $<

# How to link the product
$(PRODUCT):	$(OBJECTS) .buildmode
	$(CC) $(OBJECTS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@

# How to clean up
clean:
	$(RM) everybit *.o .buildmode *.gcov *.gcno *.gcda

test: $(PRODUCT)
	../test.py $(PRODUCT)

testquiet: $(PRODUCT)
	../test.py --quiet $(PRODUCT)

.PHONY:		all clean
