lint:
	@pylint *.[ch]*

test:
	@make
	@cd tests ; ./do_tests 2>&1 | tee results.txt
