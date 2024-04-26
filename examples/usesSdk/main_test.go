package main

import (
	"os"
	"testing"
)

// UsesSDK is a simple function that calls the SDK
func TestUserSDK(t *testing.T) {
	os.Setenv("NO_USERQUERY", "true")
	main()
}
