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
##	@cd other/15_gasHole; make test
##	@cd other/15_tracingNeed; make test
##	@cd other/23_bloom; make test
##	@cd other/25_toRust; make test
##	@cd other/27_inputSize; make test
##	@cd other/99_mappingTest; make test
	@cd tests ; make testall

rebuild:
	@cd apps/makeClass ; make ; cd -
	@cd apps/grabABI ; make ; cd -
	@cd libs ; make rebuild ; cd -
	@cd monitors ; make rebuild ; cd -
	@cd tools ; make rebuild ; cd -
	@cd tests ; make rebuild ; cd -

readmes:
	@cd apps ; make readmes
	@cd tools ; make readmes
	@cd monitors ; make readmes
	@cd libs ; make readmes
	@cd tests ; make readmes

lint:
	@cd apps ; make lint
	@cd libs ; make lint
	@cd tools ; make lint
	@cd monitors ; make lint
	@cd tests ; make lint

freshen:
	@cd monitors ; make freshen
