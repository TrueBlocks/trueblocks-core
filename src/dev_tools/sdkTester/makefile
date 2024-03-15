all: *.go
	@echo "Building the SDK..."
	@go build main.go sdk.go
	@mv main ../../../bin/sdkTester

test: *.go
	@echo "Building and testing SDK..."
	@TEST_MODE=true go run main.go sdk.go

archive:
	@echo "Archiving tests..."
	@if [ -d test ]; then \
		rm -f testing.tar.gz; \
		tar -czf testing.tar.gz test/gold; \
		echo "Tests archived into testing.tar.gz."; \
	else \
		echo "Test directory does not exist, nothing to archive."; \
	fi

extract:
	@echo "Extracting tests..."
	@if [ -f testing.tar.gz ]; then \
		rm -fR test/gold; \
		tar -xzf testing.tar.gz; \
		echo "Tests extracted."; \
	else \
		echo "Archive file testing.tar.gz does not exist, cannot extract."; \
	fi
