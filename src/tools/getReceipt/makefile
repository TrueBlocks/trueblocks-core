all:
	@echo ""

lint:
	@pylint *.[ch]*

cleanall:
	@cd $(libraries) ; make clean ; cd -
	@make clean

clean:
	-@$(RM) -f $(dest) objs/* 2> /dev/null
