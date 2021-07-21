CC := clang

TARGETS = fib transpose qsort

# CFLAGS := -Wall -g -std=gnu11
CFLAGS:= -Wall -Wextra -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings \
			-Wmissing-declarations -Wredundant-decls -Wshadow \
			-Woverloaded-virtual -g -std=gnu11
OTHER_CFLAGS ?=

LDFLAGS := # -lrt

ifeq ($(DEBUG),1)
	CFLAGS += -O0
else
	CFLAGS += -O3 -DNDEBUG
endif

ifeq ($(PARALLELIZE), 1)
	CFLAGS += -Xpreprocessor -fopenmp
	LDFLAGS += -lomp
endif

ifeq ($(ASSEMBLE),1)
	CFLAGS += -S
endif

CFLAGS += $(OTHER_CFLAGS)

# You shouldn't need to touch this.  This keeps track of whether or
# not you've changed CFLAGS.
OLD_CFLAGS := $(shell cat .cflags 2> /dev/null)
ifneq ($(CFLAGS),$(OLD_CFLAGS))
.cflags::
	@echo "$(CFLAGS)" > $@
endif


all: $(TARGETS:=.o)

%.o: %.c .cflags
	$(CC) $(CFLAGS) -c $<

fib: fib.o
	$(CC) -o fib fib.o $(LDFLAGS)

transpose: transpose.o
	$(CC) -o transpose transpose.o $(LDFLAGS)

qsort: qsort.o
	$(CC) -o qsort qsort.o $(LDFLAGS) $(LDFLAGS)

clean::
	rm -rf fib transpose qsort *.o *.s .cflags perf.data */perf.data cachegrind.out.* *.dSYM/
