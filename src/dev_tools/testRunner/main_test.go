package main

import (
	"fmt"
	"os"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func TestMainFunction(t *testing.T) {
	os.Setenv("TEST_MODE", "true")
	os.Setenv("TB_TEST_FILTER", "init")
	err := os.Chdir("../../../build/")
	if err != nil {
		logger.Fatal(fmt.Sprintf("Failed to change directory: %v", err))
	}
	wd, _ := os.Getwd()
	fmt.Println("Running from:", wd)
	main()
}
