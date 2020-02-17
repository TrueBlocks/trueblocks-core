
Step 1
	Generate random files

	Edit ./generate-random-files.sh
	Change parameters
	Run ./generate-random-files.sh

Step 2

	Run the process

	go run ./process.go ./random_files

Step 3

	Run test helper to check the values at the head of the file

	./test-helper.sh

Step 4

	Compare outpus of Test helper and the Go program
	With FLAG_OFFSET=1 the data will be in the SECOND column



ToDo:
	- Print errors to STDERR
	- Possibly read files in batches on MAX_THREADS overflow
