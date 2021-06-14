CC := clang

EXTRA_CFLAGS ?=
CFLAGS := -Wall -std=gnu99 -g

ifeq ($(DEBUG),1)
  CFLAGS += -O0
else
  CFLAGS += -O3 -DNDEBUG
endif

ifeq ($(VECTORIZE),1)
  CFLAGS += -Rpass=loop-vectorize -Rpass-missed=loop-vectorize -ffast-math
else
  CFLAGS += -fno-vectorize
endif

ifeq ($(ASSEMBLE),1)
  CFLAGS += -S
endif

ifeq ($(AVX2),1)
  CFLAGS += -mavx2
endif

CFLAGS += $(EXTRA_CFLAGS)

LDFLAGS := -lrt

# You shouldn't need to touch this.  This keeps track of whether or
# not you've changed CFLAGS.
OLD_CFLAGS := $(shell cat .cflags 2> /dev/null)
ifneq ($(CFLAGS),$(OLD_CFLAGS))
.cflags::
	@echo "$(CFLAGS)" > .cflags
endif


all: loop

%.o: %.c .cflags
	$(CC) $(CFLAGS) -c $<

loop: loop.o
	$(CC) -o loop $^ $(LDFLAGS)

clean::
	rm -f loop *.o *.s .cflags perf.data */perf.data
