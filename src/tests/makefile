all:
	@cd ../libs ; make
	@cd sha3 ; make app
	@cd speedTest ; make app
	@cd abiTest ; make app
	@cd acctTree ; make app
	@cd bytesUsed ; make app
	@cd tomlTest ; make app
	@cd stringTest ; make app

cleanall:
	@cd ../libs ; make cleanall
	@make clean

clean:
	@cd sha3 ; make clean
	@cd speedTest ; make clean
	@cd abiTest ; make clean
	@cd acctTree ; make clean
	@cd bytesUsed ; make clean
	@cd tomlTest ; make clean
	@cd stringTest ; make clean

test:
	@cd abiTest ; make test
	@cd tomlTest ; make test
	@cd stringTest ; make test

testall:
	@make test
##	@cd sha3; make test
##	@cd speedTest; make test
#	@cd acctTree ; make test
	@cd bytesUsed ; make test

readmes:
#	@cd abiTest ; abiTest -th >README.md
	@cd acctTree ; acctTree -th >README.md
	@cd sha3; sha3 -th >README.md
#	@cd speedTest ; speedTest -th >README.md
	@cd bytesUsed ; bin/bytesUsed -th >README.md
	@cd tomlTest ; bin/tomlTest -th >README.md
	@cd stringTest ; bin/stringTest -th >README.md

lint:
	@cd abiTest ; make lint
	@cd acctTree ; make lint
	@cd sha3 ; make lint
	@cd speedTest ; make lint
	@cd bytesUsed ; make lint
	@cd tomlTest ; make lint
	@cd stringTest ; make lint
