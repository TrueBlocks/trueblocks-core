all: *.go
	@echo "Building the SDK..."
	@go build main.go test_api.go test_case.go test_cmd.go test_runner.go test_sdk.go
	@mv main ../../../bin/sdkTester

test: *.go
	@echo "Building and testing SDK..."
	@TEST_MODE=true go run main.go test_api.go test_case.go test_cmd.go test_runner.go test_sdk.go

