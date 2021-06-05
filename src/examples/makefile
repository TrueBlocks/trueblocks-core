# Invoke each tool's makefile individually
all:
	@cd ../../build ; make ; cd -
	make -C forEvery
	make -C txCounter
	make -C simple

clean:
	@cd ../../build ; make clean ; cd -
	make -C forEvery clean
	make -C txCounter clean
	make -C simple clean
