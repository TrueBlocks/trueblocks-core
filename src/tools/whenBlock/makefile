lint:
	@pylint *.[ch]*

test:
	@make
	@cat tests/in.txt tests/out.txt >tests/results.txt
	@echo "Testing" >>tests/results.txt
	@whenBlock --file:tests/in.txt | tee x ; cat x >>tests/results.txt
	@echo "--------------------------------" | tee x ; cat x >>tests/results.txt
	@whenBlock --file:tests/out.txt | tee x ; cat x >>tests/results.txt ; rm -f x

