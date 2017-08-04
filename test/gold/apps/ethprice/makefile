#-------------------------------------------------
build=../../../build/apps

#-------------------------------------------------
exec=ethprice

#-------------------------------------------------
all:
	@echo ; colorEcho -t "Building $(exec)..."
	@cd $(build)/$(exec) ; make >/dev/null 2>&1

clean:
	@cd $(build)/$(exec) ; make clean

# Tools must be in the path already
test:
	@make
	@colorEcho -t "Testing $(exec)..."
	@cd tests ; ./do_tests

testall:
	@make
	@cd tests ; ./do_tests_slow
