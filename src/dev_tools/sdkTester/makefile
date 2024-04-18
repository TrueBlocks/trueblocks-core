all: *.go
	@echo "Building the SDK..."
	@go build main.go sdk.go
	@mv main ../../../bin/sdkTester

test: *.go
	@echo "Building and testing SDK..."
	@TEST_MODE=true go run main.go sdk.go
