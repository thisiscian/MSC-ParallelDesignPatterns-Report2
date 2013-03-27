NPROC=8
LIBRARY_DIR=actor_metaphor
FROG_DIR=frog
FROG_RUN=frog_exec

mpicc:
	cd $(LIBRARY_DIR) && make
	cd $(FROG_DIR) && make

gcc:
	cd $(LIBRARY_DIR) && make gcc
	cd $(FROG_DIR) && make gcc

local-%: 
	mpiexec -n $(patsubst local-%,%,$@) $(FROG_DIR)/$(FROG_RUN)

submit-%:
	./qsubmit $(patsubst submit-%,%,$@)

clean:
	@echo "Cleaning all directories..."
	@cd $(LIBRARY_DIR) && make clean
	@cd $(FROG_DIR) && make clean

