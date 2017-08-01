#-------------------------------------------------
build=../../../build/apps

#-------------------------------------------------
exec=chifra

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
	@echo "No test" | tee tests/results.txt
