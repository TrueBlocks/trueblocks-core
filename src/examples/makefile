# Invoke each tool's makefile individually
all:
	@cd ../../build ; make ; cd -
	make -C visitAll
	make -C txCounter
	make -C simple

clean:
	@cd ../../build ; make clean ; cd -
	make -C visitAll clean
	make -C txCounter clean
	make -C simple clean
