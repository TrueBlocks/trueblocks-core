package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func ParseArgs() *App {
	if len(os.Args) < 2 {
		printHelp(fmt.Errorf("not enough arguments"))
	}

	app := NewApp()

	switch os.Args[1] {
	case "--help", "-h":
		printHelp(nil)

	case "--autoname", "--delete", "--undelete", "--remove":
		if len(os.Args) != 3 {
			printHelp(fmt.Errorf("'" + os.Args[1] + "' expects an address"))
		}

		addr := os.Args[2]
		if !base.IsValidAddress(addr) {
			printHelp(fmt.Errorf("invalid address: " + addr))
		}

		app.action = Action(strings.TrimPrefix(os.Args[1], "--"))
		return app

	case "--clean", "--publish":
		app.action = Action(strings.TrimPrefix(os.Args[1], "--"))
		return app

	default:
		if len(os.Args) < 3 {
			printHelp(fmt.Errorf("not enough arguments (missing address, name, or both)"))
		}

		for _, arg := range os.Args {
			if arg == "--autoname" ||
				arg == "--delete" ||
				arg == "--undelete" ||
				arg == "--remove" ||
				arg == "--clean" ||
				arg == "--publish" {
				printHelp(fmt.Errorf("'" + arg + "', if present, must appear first in the argument list"))
			}
		}
	}

	return app
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
