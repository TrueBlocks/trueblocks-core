all:
	@cd libs ; make ; echo
	@cd apps ; make ; echo
	@cd monitors ; make ; echo
	@cd tools ; make ; echo
	@cd other/04_inerror ; make app ; echo
	@cd other/08_latestBlock ; make app ; echo
	@cd other/09_toJSON ; make app ; echo
	@cd other/15_gasHole ; make app ; echo
	@cd other/17_creations ; make app ; echo
	@cd other/18_internal ; make app ; echo
	@cd other/22_genesis ; make app ; echo
	@cd tests ; make ; echo

cleanall:
	@make clean

clean:
	@cd libs ; make clean
	@cd apps ; make clean
	@cd monitors ; make cleanall
	@cd tools ; make clean
	@cd other/04_inerror ; make clean
	@cd other/08_latestBlock ; make clean
	@cd other/09_toJSON ; make clean
	@cd other/15_gasHole ; make clean
	@cd other/17_creations ; make clean
	@cd other/18_internal ; make clean
	@cd other/22_genesis ; make clean
	@cd tests ; make clean

test:
	@make
	@clear
	@echo "Starting light tests..."
	@blockScrape -fl
	@miniBlocks freshen
	@cd apps ; make test
	@cd monitors ; make test
	@cd tools ; make test
#	@cd other/16_encodingABIs; make test
	@cd other/19_blooms; make test
	@cd tests ; make test

testall:
	@make
	@clear
	@echo "Starting heavy tests..."
	@blockScrape -fl
	@miniBlocks freshen
	@cd apps ; make testall
	@cd monitors ; make test
	@cd tools ; make testall
	@cd other/04_inerror; make test
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
	@cd tests ; make testall

rebuild:
	@cd libs ; make rebuild ; cd -
	@cd monitors ; make rebuild ; cd -

readmes:
	@cd apps ; make readmes
	@cd tools ; make readmes
	@cd monitors ; make readmes
	@cd libs ; make readmes
	@cd tests ; make readmes

lint:
	@cd apps ; make lint
	@cd tools ; make lint
	@cd monitors ; make lint
	@cd tests ; make lint

freshen:
	@cd monitors ; make freshen
