ifeq ($(CILKSAN),1)
CXXFLAGS += -fsanitize=cilk
LDFLAGS += -fsanitize=cilk
endif

ifeq ($(CILKSCALE),1)
CXXFLAGS += -mllvm -instrument-cilk -DCILKSCALE
LDFLAGS += -lcilkscale
endif

