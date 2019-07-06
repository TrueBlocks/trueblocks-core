package main

import (
	"fmt"
	"os"

	"./cmd"
)

func main() {
	cmd.Execute()
	fmt.Println("")
	os.Exit(0)
}
