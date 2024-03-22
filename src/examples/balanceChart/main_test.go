package main

import (
	"os"
	"testing"
)

func TestMain(t *testing.T) {
	os.Setenv("TB_DEBUG_CURL", "test")
	main()
}
