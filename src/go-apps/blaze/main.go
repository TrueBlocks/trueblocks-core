package main

import (
	"os"
	"./cmd"
)

func main() {
	cmd.Execute()
	os.Exit(0)
}
