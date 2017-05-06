all:
	@cd ../libs ; make
	@cd ethName ; make app
	@cd getBalance ; make
	@cd getBlock ; make
	@cd getLogs ; make
#	@cd getReceipt ; make
	@cd getTrans ; make
	@cd isContract ; make
	@cd whenBlock ; make app
	@cd scripts ; make app

cleanall:
	@cd ../libs ; make cleanall
	@make clean

clean:
	@cd ethName ; make clean
	@cd getBalance ; make clean
	@cd getBlock ; make clean
	@cd getLogs ; make clean
#	@cd getReceipt ; make clean
	@cd getTrans ; make clean
	@cd isContract ; make clean
	@cd whenBlock ; make clean

test:
	@cd ethName; make test
	@cd getBalance ; make test
	@cd getBlock ; make test
	@cd getLogs ; make test
#	@cd getReceipt ; make test
	@cd getTrans ; make test
	@cd isContract ; make test
	@cd whenBlock ; make test

testall:
	@make test

readmes:
	@cd ethName ; ethName -th >README.md
	@cd getBalance ; getBalance -th >README.md
	@cd getBlock ; getBlock -th >README.md
	@cd getLogs ; getLogs -th >README.md
#	@cd getReceipt ; getReceipt -th >README.md
	@cd getTrans ; getTrans -th >README.md
	@cd isContract ; isContract -th >README.md
	@cd whenBlock ; whenBlock -th >README.md

lint:
	@cd ethName ; make lint 2>&1 | tee ../lintReport.txt
	@cd getBalance ; make lint 2>&1 | tee -a ../lintReport.txt
	@cd getBlock ; make lint 2>&1 | tee -a ../lintReport.txt
	@cd getLogs ; make lint 2>&1 | tee -a ../lintReport.txt
	@cd getReceipt ; make lint 2>&1 | tee -a ../lintReport.txt
	@cd getTrans ; make lint 2>&1 | tee -a ../lintReport.txt
	@cd isContract ; make lint 2>&1 | tee -a ../lintReport.txt
	@cd whenBlock ; make lint 2>&1 | tee -a ../lintReport.txt
