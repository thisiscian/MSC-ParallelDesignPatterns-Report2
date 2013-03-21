NPROC=3
LIBRARY_DIR=actormetaphor
FROG_DIR=frog
FROG_RUN=frog_exec

mpicc:
	cd $(LIBRARY_DIR) && make mpicc
	cd $(FROG_DIR) && make mpicc

vampir:
	cd $(LIBRARY_DIR) && make vampir
	cd $(FROG_DIR) && make vampir

run: 
	mpiexec -n $(NPROC) $(FROG_DIR)/$(FROG_RUN)

clean:
	@echo "Cleaning all directories..."
	@cd $(LIBRARY_DIR) && make clean
	@cd $(FROG_DIR) && make clean

