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

rebuild:
	@echo "Rebuilding token lib"
	@rm -f tokenlib/*.[ch]* tokenlib/makefile tokenlib/classDefinitions/*.txt
	@grabABI TokenLib -g:tokenlib
	@echo
	@echo "Rebuilding wallet lib"
	@rm -f walletlib/*.[ch]* walletlib/makefile walletlib/classDefinitions/*.txt
	@grabABI WalletLib -g:walletlib
	@echo
