CELL := $(wildcard ./cell/*.json)
COMP := $(wildcard ./compound/*.json)

RM := rm
WATCH := ./watch.sh
BINC := binc

all:
	echo hello!

.PHONY: cell_clean
cell_clean:
	$(RM) $(CELL)

.PHONY: comp_clean
comp_clean:
	$(RM) $(COMP)

.PHONY: clean
clean:
	$(RM) $(COMP) $(CELL)

.PHONY: cell_watch
cell_watch:
	$(WATCH)

.PHONY: comp_watch
comp_watch:
	$(WATCH) comp

.PHONY: start
start:
	$(BINC) $(ARG)