UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	dest = ~/.local/share/trueblocks/cache/objs/
endif
ifeq ($(UNAME_S),Darwin)
	dest = ~/Library/Application\ Support/TrueBlocks/cache/objs/
endif

all:
	@cd simple1 && make
	@cd gasHole && make
#	@cd balances && make
#	@cd filtering && make
#	@cd recons && make
#	@cd usdprice && make
	@ls -l $(dest)

clean:
	@cd simple1 && make clean
	@cd gasHole && make clean
#	@cd balances && make clean
#	@cd filtering && make clean
#	@cd recons && make clean
#	@cd usdprice && make clean
	@ls -l $(dest)
