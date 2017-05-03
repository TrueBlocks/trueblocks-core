all:
	@cd ../libs ; make
	@cd blockScrape ; make
	@cd chifra ; make
	@cd ethprice ; make
	@cd ethslurp ; make
#	@cd getBalance ; make
	@cd getBlock ; make
	@cd getLogs ; make
#	@cd getReceipt ; make
	@cd getTrans ; make
	@cd isContract ; make
	@cd miniBlocks ; make

cleanall:
	@cd ../libs ; make clean
	@make clean

clean:
	@cd blockScrape ; make clean
	@cd chifra ; make clean
	@cd ethprice ; make clean
	@cd ethslurp ; make clean
	@cd getBalance ; make clean
	@cd getBlock ; make clean
	@cd getLogs ; make clean
	@cd getReceipt ; make clean
	@cd getTrans ; make clean
	@cd isContract ; make clean
	@cd miniBlocks ; make clean

test:
	@cd blockScrape ; make test
	@cd chifra ; make test
#	@cd getBalance ; make test
	@cd getBlock ; make test
	@cd getLogs ; make test
#	@cd getReceipt ; make test
#	@cd getTrans ; make test
	@cd isContract ; make test

testall:
	@make test
	@cd ethprice ; make test
	@cd ethslurp ; make test
	@cd miniBlocks ; make test

readmes:
	@cd blockScrape ; blockScrape -th >README.md
	@cd chifra ; chifra -th >README.md
	@cd ethprice ; ethprice -th >README.md
	@cd ethslurp ; ethslurp -th >README.md
#	@cd getBalance ; getBalance -th >README.md
	@cd getBlock ; getBlock -th >README.md
	@cd getLogs ; getLogs -th >README.md
#	@cd getReceipt ; getReceipt -th >README.md
#	@cd getTrans ; getTrans -th >README.md
	@cd isContract ; isContract -th >README.md
	@cd miniBlocks ; miniBlocks -th >README.md

lint:
	@cd blockScrape ; make lint
	@cd chifra ; make lint
	@cd ethprice ; make lint
	@cd ethslurp ; make lint
	@cd getBalance ; make lint
	@cd getBlock ; make lint
	@cd getLogs ; make lint
	@cd getReceipt ; make lint
	@cd getTrans ; make lint
	@cd isContract ; make lint
	@cd miniBlocks ; make lint
