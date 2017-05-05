all:
	@cd ../libs ; make
	@cd cacheMan ; make app
	@cd ethname ; make app
	@cd whenBlock ; make app
#	@cd getBalance ; make
	@cd getBlock ; make
	@cd getLogs ; make
#	@cd getReceipt ; make
	@cd getTrans ; make
	@cd isContract ; make
	@cd scripts ; make app

cleanall:
	@cd ../libs ; make cleanall
	@make clean

clean:
	@cd cacheMan; make clean
	@cd ethname ; make clean
	@cd whenBlock ; make clean
#	@cd getBalance ; make clean
	@cd getBlock ; make clean
	@cd getLogs ; make clean
#	@cd getReceipt ; make clean
	@cd getTrans ; make clean
	@cd isContract ; make clean

test:
	@cd cacheMan ; make test
	@cd whenBlock ; make test
#	@cd getBalance ; make test
	@cd getBlock ; make test
	@cd getLogs ; make test
#	@cd getReceipt ; make test
	@cd getTrans ; make test
	@cd isContract ; make test

testall:
	@make test
#	@cd ethname; make test

readmes:
	@cd cacheMan ; cacheMan -th >README.md
	@cd ethname ; ehtname -th >README.md
	@cd whenBlock ; whenBlock -th >README.md
	@cd getBalance ; getBalance -th >README.md
	@cd getBlock ; getBlock -th >README.md
	@cd getLogs ; getLogs -th >README.md
	@cd getReceipt ; getReceipt -th >README.md
	@cd getTrans ; getTrans -th >README.md
	@cd isContract ; isContract -th >README.md

lint:
	@cd cacheMan ; make lint
	@cd ethname ; make lint
	@cd whenBlock ; make lint
	@cd getBalance ; make lint
	@cd getBlock ; make lint
	@cd getLogs ; make lint
	@cd getReceipt ; make lint
	@cd getTrans ; make lint
	@cd isContract ; make lint
