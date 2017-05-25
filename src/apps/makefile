all:
	@cd ../libs ; make
	@cd blockScrape ; make
	@cd chifra ; make
	@cd ethprice ; make
	@cd ethslurp ; make
	@cd grabABI ; make
	@cd makeClass ; make
	@cd miniBlocks ; make

cleanall:
	@cd ../libs ; make clean
	@make clean

clean:
	@cd blockScrape ; make clean
	@cd chifra ; make clean
	@cd ethprice ; make clean
	@cd ethslurp ; make clean
	@cd grabABI ; make clean
	@cd makeClass ; make clean
	@cd miniBlocks ; make clean

test:
	@cd blockScrape ; make test
	@cd chifra ; make test

testall:
	@make test
	@cd ethprice ; make test
	@cd ethslurp ; make test
	@cd grabABI ; make test
	@cd makeClass ; make test
	@cd miniBlocks ; make test

readmes:
	@cd blockScrape ; blockScrape -th >README.md
	@cd chifra ; chifra -th >README.md
	@cd ethprice ; ethprice -th >README.md
	@cd ethslurp ; ethslurp -th >README.md
	@cd grabABI ; grabABI -th >README.md
	@cd makeClass ; makeClass -th >README.md
	@cd miniBlocks ; miniBlocks -th >README.md

lint:
	@cd blockScrape ; make lint
	@cd chifra ; make lint
	@cd ethprice ; make lint
	@cd ethslurp ; make lint
	@cd grabABI ; make lint
	@cd makeClass ; make lint
	@cd miniBlocks ; make lint
