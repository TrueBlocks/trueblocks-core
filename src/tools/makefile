all:
	@cd ../libs ; make
	@cd grabABI ; make app
	@cd sha3 ; make app
	@cd whenBlock ; make app
	@cd cacheMan ; make app
	@cd ethname ; make app
	@cd makeClass ; make app
	@cd speedTest ; make app
	@cd abiTest ; make app
	@cd acctTree ; make app
	@cd scripts ; make app

cleanall:
	@cd ../libs ; make cleanall
	@make clean

clean:
	@cd grabABI ; make clean
	@cd sha3 ; make clean
	@cd whenBlock ; make clean
	@cd cacheMan; make clean
	@cd ethname ; make clean
	@cd makeClass ; make clean
	@cd speedTest ; make clean
	@cd abiTest ; make clean

test:
	@cd grabABI; make test
	@cd whenBlock ; make test
	@cd cacheMan ; make test
	@cd makeClass; make test
	@cd abiTest ; make test

testall:
	@make test
##	@cd sha3; make test
##	@cd ethname; make test
##	@cd speedTest; make test
#	@cd acctTree ; make test

readmes:
	@cd grabABI ; grabABI -th >README.md
	@cd sha3; sha3 -th >README.md
	@cd whenBlock ; whenBlock -th >README.md
	@cd cacheMan ; cacheMan -th >README.md
#	@cd ethname ; ehtname -th >README.md
	@cd makeClass ; makeClass -th >README.md
#	@cd speedTest ; speedTest -th >README.md
	@cd acctTree ; acctTree -th >README.md
#	@cd abiTest ; abiTest -th >README.md

lint:
	@cd abiTest ; make lint
	@cd acctTree ; make lint
	@cd cacheMan ; make lint
	@cd ethname ; make lint
	@cd grabABI ; make lint
	@cd makeClass ; make lint
	@cd sha3 ; make lint
	@cd speedTest ; make lint
	@cd whenBlock ; make lint
