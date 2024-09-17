package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

func main() {
	allUsages := getDiskUsage()

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
	file.StringToAsciiFile(outputFile, screen)

	if msg, err := copyToRemote(outputFile, "docs", "unchainedindex.io", "/var/www/unchainedindex.io/html/"); err != nil {
		fmt.Printf("Failed to copy file to remote host: %v\n", err)
	} else {
		fmt.Print(msg)
	}

	fmt.Printf("Disk usage report generated at %s\n", outputFile)
}
