CC := clang

TARGETS = example1 example2 example3 example4

OTHER_CFLAGS ?=
CFLAGS := -Wall -g -std=gnu99

ifeq ($(DEBUG),1)
  CFLAGS += -O0
else
  CFLAGS += -O3
endif

V ?= 1

ifeq ($(VECTORIZE),1)
  #CFLAGS += -ftree-vectorizer-verbose=$(V)
  CFLAGS += -Rpass=loop-vectorize -Rpass-missed=loop-vectorize 
else
  CFLAGS += -fno-vectorize
endif

ifeq ($(ASSEMBLE),1)
  CFLAGS += -S
endif

ifeq ($(AVX2),1)
  CFLAGS += -mavx2
endif

CFLAGS += $(OTHER_CFLAGS)

LDFLAGS := -lrt

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

%.lst: %.c .cflags
	$(CC) $(CFLAGS) -Wa,-aslh -c $< > $@

clean::
	rm -rf 
	rm -f *.o *.s .cflags perf.data *.lst
