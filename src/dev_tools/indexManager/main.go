package main

import (
	"fmt"
	"os"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

func main() {
	allUsages, err := getDiscUsage()
	if err != nil {
		fmt.Printf("Error returned from getDiscUsage: %v\n", err)
		return
	}

	screen := getTitle()
	screen += headerRow()
	var lastMachine string
	for _, usage := range allUsages {
		line, machine := dataRow(&usage, lastMachine)
		screen += line
		lastMachine = machine
	}
	screen += blankRow()
	screen += legendRow()

	outputFile := "disc_usage.html"
	_ = file.StringToAsciiFile(outputFile, screen)

	now := time.Now()
	ts := now.Format("2006-01-02 15:04:05")
	if msg, err := copyToRemote(outputFile, "docs", "unchainedindex.io", "/var/www/unchainedindex.io/html/"); err != nil {
		fmt.Printf("%s: Failed to copy file to remote host: %v\n", ts, err)
	} else {
		fmt.Printf("%s: %s", ts, msg)
	}
	os.Remove(outputFile)
	fmt.Printf("%s: Report generated.\n", ts)
}
