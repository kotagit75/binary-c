
PROG := ../../public/binc_env
SRCS := $(wildcard *.c) $(wildcard ../lib/*.c)
OBJS := $(SRCS:%.c=%.o)
DEPS := $(SRCS:%.c=%.d)
CC := gcc

all: $(OBJS) $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $^ -ljansson

.c.o:
	$(CC) -MMD -MP -MF $(<:%.c=%.d) -c $< -o $(<:%.c=%.o)

.PHONY: clean
clean:
	$(RM) $(PROG) $(OBJS) $(DEPS)

-include $(DEPS)