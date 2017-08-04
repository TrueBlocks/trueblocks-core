#-------------------------------------------------
build=../../../build/apps

#-------------------------------------------------
exec=miniBlocks

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
	@echo "No slow tests, use make testall"

testall:
	@make
	@$(exec) -lv | tee tests/goldList.txt
	@$(exec) -lvt | tee tests/fullList.txt
	@cd tests ; split -l20000 fullList.txt ; rm -f fullList.txt
