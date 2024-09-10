package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func parseArgs() (ok bool, action Action) {
	if len(os.Args) < 2 {
		printError("not enough arguments")
		printHelp()
		return false, ""
	}

	switch os.Args[1] {
	case "--help", "-h":
		printHelp()
		return false, ""
	case "--autoname", "--delete", "--undelete", "--remove":
		if len(os.Args) != 3 {
			printError("'" + os.Args[1] + "' accepts only a single argument: address")
			printHelp()
			return false, ""
		}
		addr := os.Args[2]
		if !base.IsValidAddress(addr) {
			printError("Invalid address: " + addr)
			printHelp()
			return false, ""
		}
		return true, Action(strings.TrimPrefix(os.Args[1], "--"))

	case "--clean", "--pin", "--publish":
		return true, Action(strings.TrimPrefix(os.Args[1], "--"))

	default:
		if len(os.Args) < 3 {
			printError("not enough arguments (missing address, name, or both)")
			printHelp()
			return false, ""
		}
		for _, arg := range os.Args {
			if arg == "--autoname" || arg == "--delete" || arg == "--undelete" || arg == "--remove" {
				printError("'" + arg + "', if present, must appear first in the argument list")
				printHelp()
				return false, ""
			}
		}
		return true, ""
	}
}

func getValue(index int, field, def string) (base.Address, string, uint64) {
	var value string
	prompt := "Enter " + field + " : "
	if len(os.Args) > index {
		value = os.Args[index]
		fmt.Println(prompt, value)
	} else if len(field) > 0 {
		value = readInput(prompt, def)
	} else {
		value = def
	}
	return base.HexToAddress(value), value, base.MustParseUint64(value)
}

func readInput(prompt string, defaultValue string) string {
	reader := bufio.NewReader(os.Stdin)
	fmt.Print(prompt)
	input, _ := reader.ReadString('\n')
	input = strings.TrimSpace(input)
	if input == "" {
		return defaultValue
	}
	return input
}

const (
	defSource = "TrueBlocks.io"
	defTags   = "99-User-Defined"
)
