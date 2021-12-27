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
	@cd balances && make
	@cd usdprice && make
#	@cd recons && make
	@ls -l $(dest)

clean:
	@cd simple1 && make clean
	@cd gasHole && make clean
	@cd balances && make clean
	@cd usdprice && make clean
#	@cd recons && make clean
	@ls -l $(dest)

test:
	@mkdir -p testing
	@make all
	@echo "Testing simple1"
	@chifra export --load simple1 0xf503017d7baf7fbc0fff7492b751025c6a78179b --last_block 11500000 >testing/working 2>/dev/null
	@echo "Testing gasHole"
	@chifra export --load gasHole 0xf503017d7baf7fbc0fff7492b751025c6a78179b --last_block 11500000 >>testing/working 2>/dev/null
	@echo "Testing balances"
	@chifra export --load balances 0xf503017d7baf7fbc0fff7492b751025c6a78179b --last_block 11500000 >>testing/working 2>/dev/null
	@echo "Testing usdprice"
	@chifra export --load usdprice 0xf503017d7baf7fbc0fff7492b751025c6a78179b --last_block 11500000 >>testing/working 2>/dev/null
	@diff testing/working testing/gold
