package main

import (
	"os"
	"testing"
)

// TestFuzzer is a simple function that calls the SDK
func TestFuzzer(t *testing.T) {
	os.Setenv("TB_TEST_FILTER", "export")
	main()
}
