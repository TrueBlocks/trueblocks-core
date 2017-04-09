all:
	@cd ../libs ; make ; echo
	@cd grabABI ; make app ; echo
	@cd sha3 ; make app ; echo
	@cd whenBlock ; make app ; echo
	@cd cacheMan ; make app ; echo
	@cd ethname ; make app ; echo
	@cd makeClass ; make app ; echo
	@cd speedTest ; make app ; echo

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

test:
	@cd grabABI; make test
	@cd whenBlock ; make test
	@cd cacheMan ; make test
	@cd makeClass; make test

testall:
	@make test
##	@cd sha3; make test
##	@cd ethname; make test
##	@cd speedTest; make test

readmes:
	@cd grabABI ; grabABI -th >README.md
	@cd sha3; sha3 -th >README.md
	@cd whenBlock ; whenBlock -th >README.md
	@cd cacheMan ; cacheMan -th >README.md
#	@cd ethname ; ehtname -th >README.md
	@cd makeClass ; makeClass -th >README.md
#	@cd speedTest ; scrapeTest -th >README.md
