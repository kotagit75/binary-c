
PROG := ../../model/tool/org2elf
SRCS := $(wildcard *.c) $(wildcard ../lib/*.c)
OBJS := $(SRCS:%.c=%.o)
DEPS := $(SRCS:%.c=%.d)

CC := gcc

all: $(DEPENDS) $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $^ -ljansson

.cpp.o:
	$(CC) -MMD -MP -MF $(<:%.c=%.d) -c $< -o $(<:%.c=%.o)

.PHONY: clean
clean:
	$(RM) $(PROG) $(OBJS) $(DEPS)

-include $(DEPS)