all:
	@cd ../libs ; make
	@cd grabABI ; make app
	@cd whenBlock ; make app
	@cd cacheMan ; make app
	@cd ethname ; make app
	@cd makeClass ; make app
	@cd scripts ; make app

cleanall:
	@cd ../libs ; make cleanall
	@make clean

clean:
	@cd grabABI ; make clean
	@cd whenBlock ; make clean
	@cd cacheMan; make clean
	@cd ethname ; make clean
	@cd makeClass ; make clean

test:
	@cd grabABI; make test
	@cd whenBlock ; make test
	@cd cacheMan ; make test
	@cd makeClass; make test

testall:
	@make test
##	@cd ethname; make test

readmes:
	@cd grabABI ; grabABI -th >README.md
	@cd whenBlock ; whenBlock -th >README.md
	@cd cacheMan ; cacheMan -th >README.md
#	@cd ethname ; ehtname -th >README.md
	@cd makeClass ; makeClass -th >README.md

lint:
	@cd cacheMan ; make lint
	@cd ethname ; make lint
	@cd grabABI ; make lint
	@cd makeClass ; make lint
	@cd whenBlock ; make lint
