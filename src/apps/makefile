all:
	@cd ../libs ; make
	@cd blockScrape ; make ; echo
	@cd chifra ; make ; echo
	@cd ethprice ; make ; echo
	@cd ethslurp ; make ; echo
#	@cd getBalance ; make ; echo
	@cd getBlock ; make ; echo
#	@cd getCode ; make ; echo
	@cd getLogs ; make ; echo
#	@cd getReceipt ; make ; echo
	@cd getTrans ; make ; echo
	@cd miniBlocks ; make ; echo

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
	@cd getCode ; make clean
	@cd getLogs ; make clean
	@cd getReceipt ; make clean
	@cd getTrans ; make clean
	@cd miniBlocks ; make clean

test:
#	@cd blockScrape ; make test
	@cd chifra ; make test
#	@cd getBalance ; make test
	@cd getBlock ; make test
#	@cd getCode ; make test
	@cd getLogs ; make test
#	@cd getReceipt ; make test
#	@cd getTrans ; make test

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
#	@cd getCode ; getCode -th >README.md
	@cd getLogs ; getLogs -th >README.md
#	@cd getReceipt ; getReceipt -th >README.md
#	@cd getTrans ; getTrans -th >README.md
	@cd miniBlocks ; miniBlocks -th >README.md
