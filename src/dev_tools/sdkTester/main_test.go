package main

import (
	"fmt"
	"log"
	"os"
	"testing"
)

func TestMainFunction(t *testing.T) {
	err := os.Chdir("../../../build/")
	if err != nil {
		logger.Fatalf("Failed to change directory: %v", err)
	}
	wd, _ := os.Getwd()
	fmt.Println("Running from:", wd)
	main()
}
