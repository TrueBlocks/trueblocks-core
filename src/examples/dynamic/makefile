#-------------------------------------------------
libs=/usr/local/lib/tb/*.a
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    libs += -lcurl
    dest = ~/.local/share/trueblocks/$(cacheDest)
    out = lib$(dyno).so
endif
ifeq ($(UNAME_S),Darwin)
    libs += /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib/libcurl.4.tbd
    dest = ~/Library/Application\ Support/TrueBlocks/$(cacheDest)
    out = lib$(dyno).dylib
endif


cacheDest=cache/mainnet/objs/
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	dest = ~/.local/share/trueblocks/$(cacheDest)
endif
ifeq ($(UNAME_S),Darwin)
	dest = ~/Library/Application\ Support/TrueBlocks/$(cacheDest)
endif

all:
	@cd simple1 && make
	@cd gasHole && make
	@cd balances && make
	@cd usdprice && make
	@cd recirc && make
#	@cd recons && make
	@ls -l $(dest)

clean:
	@cd simple1 && make clean
	@cd gasHole && make clean
	@cd balances && make clean
	@cd usdprice && make clean
	@cd recirc && make clean
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
	@echo "Testing recirc"
	@chifra export --load recirc 0xf503017d7baf7fbc0fff7492b751025c6a78179b --last_block 11500000 >>testing/working 2>/dev/null
	@diff testing/working testing/gold
