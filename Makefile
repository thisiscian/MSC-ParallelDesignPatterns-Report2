NPROC=2
LIBRARY_DIR=actormetaphor
TEST_DIR=test
TEST_RUN=test_exec
FROG_DIR=frog
FROG_RUN=frog_exec

all: library test frog

library:
	cd $(LIBRARY_DIR) && make

test: library
	cd $(TEST_DIR) && make $(TEST_RUN)

run_test: test
	mpiexec -n $(NPROC) ./$(TEST_RUN)

frog: library
	cd $(FROG_DIR) && make $(FROG_RUN)

run_frog: frog
	mpiexec -n $(NPROC) ./$(FROG_RUN)

clean:
	@echo "Cleaning all directories..."
	@cd $(LIBRARY_DIR) && make clean
	@cd $(FROG_DIR) && make clean
	@cd $(TEST_DIR) && make clean

