all:
	@cd libs ; make ; echo
	@cd apps ; make ; echo
	@cd tools ; make ; echo
#	@cd monitors ; make ; echo
#	@cd monitors/dao ; make ; echo
#	@cd monitors/singular ; make ; echo
	@cd other/04_inerror ; make app ; echo
	@cd other/07_bytesUsed ; make app ; echo
	@cd other/08_latestBlock ; make app ; echo
	@cd other/09_toJSON ; make app ; echo
	@cd other/15_gasHole ; make app ; echo
	@cd other/17_creations ; make app ; echo
	@cd other/18_internal ; make app ; echo
	@cd other/22_genesis ; make app ; echo
	@cd other/stringTest; make app ; echo

cleanall:
	@make clean

clean:
	@cd libs ; make clean
	@cd apps ; make clean
	@cd tools ; make clean
#	@cd monitors/dao ; make cleanall
#	@cd monitors/singular ; make cleanall
	@cd other/04_inerror ; make clean
	@cd other/07_bytesUsed ; make clean
	@cd other/08_latestBlock ; make clean
	@cd other/09_toJSON ; make clean
	@cd other/15_gasHole ; make clean
	@cd other/17_creations ; make clean
	@cd other/18_internal ; make clean
	@cd other/22_genesis ; make clean
	@cd other/stringTest; make clean

test:
	@make
	@clear
	@echo "Starting light tests..."
	@blockScrape -fl
	@miniBlocks freshen
	@cd apps ; make test
	@cd tools ; make test
#	@cd monitors/dao; make test
#	@cd monitors/singular; make test
#	@cd other/16_encodingABIs; make test
	@cd other/19_blooms; make test
	@cd other/stringTest; make test

testall:
	@make
	@clear
	@echo "Starting heavy tests..."
	@blockScrape -fl
	@miniBlocks freshen
	@cd apps ; make testall
	@cd tools ; make testall
	@cd other/04_inerror; make test
##	@cd other/07_bytesUsed; make test
	@cd other/18_internal; make test
##	@cd other/05_visitor; make test
##	@cd other/06_addresses; make test
##	@cd other/08_latestBlock; make test
##	@cd other/15_gasHole; make test
##	@cd other/15_tracingNeed; make test
##	@cd other/23_bloom; make test
##	@cd other/25_toRust; make test
##	@cd other/27_inputSize; make test
##	@cd other/99_mappingTest; make test

rebuild:
	@cd monitors/dao ; make rebuild ; cd -
	@cd monitors/singular ; make rebuild ; cd -
	@cd monitors/ens ; make rebuild ; cd -
	@cd libs ; make rebuild ; cd -
	@makeClass -ar

readmes:
	@cd apps ; make readmes
	@cd tools ; make readmes
#	@cd monitors/tokenlib ; cat README.md
#	@cd monitors/walletlib ; cat README.md
#	@cd libs/abilib ; cat README.md
#	@cd libs/etherlib ; cat README.md
#	@cd libs/utillib ; cat README.md

lint:
	@cd apps ; make lint
	@cd tools ; make lint
