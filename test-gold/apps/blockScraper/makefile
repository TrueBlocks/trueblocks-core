#-------------------------------------------------
build=../../../build/apps

#-------------------------------------------------
exec=blockScrape

#-------------------------------------------------
all:
	@echo ; colorEcho -t "Building $(exec)..."
	@cd $(build)/$(exec) ; make >/dev/null 2>&1
	@echo ; colorEcho -t "Building bytesUsed..."
	@cd bytesUsed ; make

clean:
	@cd $(build)/$(exec) ; make clean
	@cd bytesUsed ; make clean

# Tools must be in the path already
test:
	@make
	@colorEcho -t "Testing $(exec)..."
	@echo "Use testall for tests on this folder"

testall:
	@make test
	@$(exec) -vls | tee tests/goldList.txt
	@$(exec) -vl | tee tests/fullList.txt
	@cd tests ; split -l20000 fullList.txt ; rm -f fullList.txt
	@cd bytesUsed ; make test
