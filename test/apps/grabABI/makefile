#-------------------------------------------------
build=../../../build/apps

#-------------------------------------------------
exec=grabABI

#-------------------------------------------------
all:
	@echo ; colorEcho -t "Building $(exec)..."
	@cd $(build)/$(exec) ; make >/dev/null 2>&1

clean:
	@cd $(build)/$(exec) ; make clean

# Tools must be in the path already
testall:
test:
	@make
	@colorEcho -t "Testing $(exec)..."
	@cd tests ; ./do_tests | tee results.txt
