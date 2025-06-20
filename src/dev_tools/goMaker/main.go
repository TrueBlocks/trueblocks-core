package main

import (
	"embed"
	"fmt"
	"io"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/goMaker/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

//go:embed help.txt help_verbose.txt
var helpFS embed.FS

func showHelp() {
	helpFileName := "help.txt"
	if types.IsVerbose() {
		helpFileName = "help_verbose.txt"
	}

	helpFile, err := helpFS.Open(helpFileName)
	if err != nil {
		fmt.Println("Error: Could not load help text.")
		return
	}
	defer helpFile.Close()

	helpContent, err := io.ReadAll(helpFile)
	if err != nil {
		fmt.Println("Error: Could not read help text.")
		return
	}

	fmt.Println(string(helpContent))
}

func main() {
	showHelpFlag := false
	for _, arg := range os.Args {
		if arg == "--help" || arg == "-h" || arg == "-help" || arg == "help" {
			showHelpFlag = true
		}
		if arg == "--verbose" || arg == "-v" || arg == "-verbose" {
			types.SetVerbose(true)
		}
	}

	if showHelpFlag {
		showHelp()
		return
	}

	// Normal execution

	// First check if templates folder exists and isn't empty
	if err := types.ValidateTemplatesFolder(); err != nil {
		fmt.Println("Error:", err)
		fmt.Println("\nHere are the requirements to run goMaker:")
		types.SetVerbose(false)
		showHelp()
		os.Exit(1)
	}

	codeBase, err := types.LoadCodebase()
	if err != nil {
		if strings.Contains(err.Error(), "could not find the templates directory") {
			fmt.Println("Error:", err)
			fmt.Println("\nHere are the requirements to run goMaker:")
			types.SetVerbose(false)
			showHelp()
			os.Exit(1)
		}
		logger.Fatal(err)
	}
	codeBase.Generate()
}
