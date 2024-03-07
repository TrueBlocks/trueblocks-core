all: *.go
	@echo "building the sdk"
	@go build main.go sdk.go
	@mv main ../../../bin/sdkTester

test: *.go
	@echo "building and testing sdk"
	@TEST_MODE=true go run main.go sdk.go
