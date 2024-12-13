#-------------------------------------------------
bin=bin

#-------------------------------------------------
exec=keystore
dest=$(bin)/$(exec)

#-------------------------------------------------
all:
	@make app

every:
	@cd ../../build ; make ; cd -
	@make app

app:
	@mkdir -p bin
	go build -o $(dest) .

test:
	@make
	@echo "No tests"

#-------------------------------------------------
clean:
	-@$(RM) -f $(dest)
