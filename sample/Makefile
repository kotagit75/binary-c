MAKE := make
samples := _sample_debug _sample_init_comp _sample_init_cell _sample_exit0_comp _sample_exit0_cell _sample_putA_comp _sample_putA_cell

all:
	@$(foreach val, $(samples), $(MAKE) -C $(val);)

.PHONY: clean
clean:
	@$(foreach val, $(samples), $(MAKE) -C $(val) clean;)