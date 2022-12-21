#-------------------------------------------------
bin=bin

#-------------------------------------------------
exec=four_bytes
dest=$(bin)/$(exec)

#-------------------------------------------------
all:
	@make app

every:
	@cd ../../../build ; make ; cd -
	@make app

app:
	@mkdir -p bin
	@mv go.mod.save go.mod
	@mv go.sum.save go.sum
	go build -o $(dest) .
	@mv go.mod go.mod.save
	@mv go.sum go.sum.save

test:
	@make
	@echo "No tests"

#-------------------------------------------------
clean:
	-@$(RM) -f $(dest)
