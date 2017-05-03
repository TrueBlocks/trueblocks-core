all:
	@cd utillib; make
	@cd abilib; make
	@cd etherlib; make

tokens:
	@make all
	@cd tokenlib; make
	@cd walletlib; make

cleanall:
	@make clean

clean:
	@cd utillib; make clean
	@cd abilib; make clean
	@cd etherlib; make clean
	@cd tokenlib; make clean
	@cd walletlib; make clean

readmes:
	@echo ""
#	@echo "Nothing here yet"

rebuild:
	@cd abilib ; make rebuild
	@cd etherlib ; make rebuild
	@cd tokenlib ; make rebuild
	@cd walletlib ; make rebuild
