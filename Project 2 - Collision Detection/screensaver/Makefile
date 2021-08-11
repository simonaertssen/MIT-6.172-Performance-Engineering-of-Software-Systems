# Makefile for screensaver
# Copyright (c) 2012 the Massachusetts Institute of Technology
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


# This Makefile is set up to build screensaver in one of two modes.
# In debug mode, the C compiler does virtually no optimizations
# (-O0) and embeds a bunch of debugging symbols to make work with GDB easier.
# In release mode, the compiler will do every optimization it can (-O3) and keep
# debugging symbols out; this should decrease the size of the final binary and
# make it perform better.
#
# To compile in debug mode, type "make DEBUG=1".  To to compile in release
# mode, type "make DEBUG=0" or simply "make".
#
# If you type "make prof", Make will instrument the output for profiling with
# gprof.  Be sure you run "make clean" first!
#
# If everything gets wacky and you need a sane place to start from, you can
# type "make clean", which will remove all compiled code.
#
# If you want to do something wacky with your compiler flags--like enabling
# debug symbols but keeping optimizations on--you can specify CXXFLAGS or
# LDFLAGS on the command line.  If you want to use a predefined mode but augment
# the predefined CXXFLAGS or LDFLAGS, you can specify EXTRA_CXXFLAGS or
# EXTRA_LDFLAGS on the command line.


# The sources we're building
HEADERS = $(wildcard *.h)
PRODUCT_SOURCES = $(filter-out graphic_stuff.c, $(wildcard *.c))

# What we're building
PRODUCT_OBJECTS = $(PRODUCT_SOURCES:.c=.o)
PRODUCT = screensaver
PROFILE_PRODUCT = $(PRODUCT:%=%.prof) #the product, instrumented for gprof

# What we're building with
CXX = clang
CXXFLAGS = -std=gnu99 -Wall -ftapir
LDFLAGS = -lrt -lm -lcilkrts

include ./cilkutils.mk

# Determine which profile--debug or release--we should build against, and set
# CFLAGS appropriately.

ifeq ($(DEBUG),1)
  # We want debug mode.
  CXXFLAGS += -g -O0 -gdwarf-3
else
  # We want release mode.
  CXXFLAGS += -O3 -DNDEBUG
endif


# By default, make the product.
all:		$(PRODUCT)

# How to build for profiling
prof:		$(PROFILE_PRODUCT)

lint:
	python clint.py *.h *.c


# How to clean up
clean:
	$(RM) $(PRODUCT) $(PROFILE_PRODUCT) *.o *.out


# How to compile a C file
%.o:		%.c $(HEADERS)
	$(CXX) $(CXXFLAGS) $(EXTRA_CXXFLAGS) -o $@ -c $<

# How to link the product
$(PRODUCT): LDFLAGS += -lXext -lX11
$(PRODUCT):	$(PRODUCT_OBJECTS) graphic_stuff.o
	$(CXX) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@ $(PRODUCT_OBJECTS) graphic_stuff.o

# How to build the product, instrumented for profiling
$(PROFILE_PRODUCT): CXXFLAGS += -DPROFILE_BUILD -pg
$(PROFILE_PRODUCT): LDFLAGS += -pg
$(PROFILE_PRODUCT): $(PRODUCT_OBJECTS)
	$(CXX)  $(PRODUCT_OBJECTS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $(PROFILE_PRODUCT)
