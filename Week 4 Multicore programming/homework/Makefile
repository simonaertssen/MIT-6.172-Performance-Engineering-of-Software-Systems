CC := clang

TARGETS = queens

CFLAGS := -Wall -g -std=gnu11
#CFLAGS:= -Wall -Wextra -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings \
			-Wmissing-declarations -Wredundant-decls -Wshadow \
			-Woverloaded-virtual -g -std=gnu11
OTHER_CFLAGS ?=

LDFLAGS := # -lrt

ifeq ($(DEBUG),1)
	CFLAGS += -O0
else
	CFLAGS += -O3 -DNDEBUG
endif

ifeq ($(PARALLEL), 1)
	CFLAGS += -Xpreprocessor -fopenmp
	LDFLAGS += -lomp
endif

ifeq ($(ASSEMBLE),1)
	CFLAGS += -S
endif

CFLAGS += $(OTHER_CFLAGS)

# You shouldn't need to touch this. Keeps track of whether or not you've changed
# CFLAGS.
OLD_CFLAGS := $(shell cat .cflags 2> /dev/null)
ifneq ($(CFLAGS),$(OLD_CFLAGS))
.cflags::
	@echo "$(CFLAGS)" > $@
endif


all: $(TARGETS:=.o)

%.o: %.c .cflags
	$(CC) $(CFLAGS) -c $<

queens: queens.o
	$(CC) -o queens queens.o $(LDFLAGS) $(LDFLAGS)

reductions: reductions.o
	$(CC) -o reductions reductions.o $(LDFLAGS) $(LDFLAGS)

clean::
	rm -rf queens reductions *.o *.s .cflags perf.data */perf.data cachegrind.out.* *.dSYM/
