NPROC=8
LIBRARY_DIR=actor_metaphor
FROG_DIR=frog
FROG_RUN=frog_exec

mpicc:
	cd $(LIBRARY_DIR) && make
	cd $(FROG_DIR) && make

library:
	cd $(LIBRARY_DIR) && make

frog:
	cd $(FROG_DIR) && make

run:
	mpiexec -n 1 $(FROG_DIR)/$(FROG_RUN)

local-%: 
	mpiexec -n $(patsubst local-%,%,$@) $(FROG_DIR)/$(FROG_RUN)

submit-%:
	./qsubmit $(patsubst submit-%,%,$@)

clean:
	@cd $(LIBRARY_DIR) && make clean
	@cd $(FROG_DIR) && make clean

