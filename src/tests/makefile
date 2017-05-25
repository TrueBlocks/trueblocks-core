all:
	@cd ../libs ; make
	@cd abiTest ; make
	@cd acctTree ; make
	@cd bytesUsed ; make
	@cd jsonTest ; make
	@cd sha3 ; make
	@cd speedTest ; make
	@cd stringTest ; make
	@cd tomlTest ; make

cleanall:
	@cd ../libs ; make cleanall
	@make clean

clean:
	@cd abiTest ; make clean
	@cd acctTree ; make clean
	@cd bytesUsed ; make clean
	@cd jsonTest ; make clean
	@cd sha3 ; make clean
	@cd speedTest ; make clean
	@cd stringTest ; make clean
	@cd tomlTest ; make clean

test:
	@cd abiTest ; make test
	@cd jsonTest ; make test
	@cd stringTest ; make test
	@cd tomlTest ; make test

testall:
	@make test
#	@cd acctTree ; make test
#	@cd bytesUsed ; make test
##	@cd sha3; make test
##	@cd speedTest; make test

readmes:
#	@cd abiTest ; abiTest -th >README.md
	@cd acctTree ; acctTree -th >README.md
	@cd bytesUsed ; bin/bytesUsed -th >README.md
	@cd jsonTest ; bin/jsonTest -th >README.md
	@cd sha3; sha3 -th >README.md
#	@cd speedTest ; speedTest -th >README.md
	@cd stringTest ; bin/stringTest -th >README.md
	@cd tomlTest ; bin/tomlTest -th >README.md

lint:
	@cd abiTest ; make lint
	@cd acctTree ; make lint
	@cd bytesUsed ; make lint
	@cd jsonTest ; make lint
	@cd sha3 ; make lint
	@cd speedTest ; make lint
	@cd stringTest ; make lint
	@cd tomlTest ; make lint
