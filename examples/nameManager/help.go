package main

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
)

func printHelp() {
	helpText := `
Usage: tool [options] address name [tags] [source] [symbol] [decimals]
       tool --autoname address
       tool --delete address
       tool --undelete address
       tool --remove address

Required Arguments:
  address      The address (string)
  name         The name (string)

Optional Arguments:
  tags         The tags (string, default: "")
  source       The source (string, default: "")
  symbol       The symbol (string, default: "")
  decimals     The decimals (uint64, default: 0)

Options:
  --help       Show this help message and exit
  --autoname   Read the chain for the ERC20 values and set the name
  --delete     Delete the name entry for the given address
  --undelete   Undelete the name entry for the given address
  --remove     Remove the node for the given address
`
	fmt.Fprintf(os.Stderr, "%s", helpText)
	os.Exit(0)
}

func printError(msg string) {
	fmt.Fprintf(os.Stderr, "%sError: %s%s\n", colors.Red, msg, colors.Off)
}
