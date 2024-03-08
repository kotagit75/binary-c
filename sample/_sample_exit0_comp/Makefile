
PROG := ../../model/tool/sample_exit0_comp
SRCS := $(wildcard ../../src/lib/*.c) $(wildcard *.c)
OBJS := $(SRCS:%.c=%.o)
DEPS := $(SRCS:%.c=%.d)

CC := gcc

all: $(DEPENDS) $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $^ -ljansson -O3 -mtune=native -march=native -mfpmath=both

.c.o:
	$(CC) -MMD -MP -MF $(<:%.c=%.d) -c $< -o $(<:%.c=%.o)

.PHONY: clean
clean:
	$(RM) $(PROG) $(OBJS) $(DEPS)

-include $(DEPS)