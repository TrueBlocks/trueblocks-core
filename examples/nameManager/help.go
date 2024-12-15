package main

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
)

func printHelp(err error) {
	helpText := `
Usage: nameManager [options] address name [tags [source [symbol [decimals]]]]
       nameManager --autoname address
       nameManager --delete address
       nameManager --undelete address
       nameManager --remove address
       nameManager --clean
       nameManager --publish

Required Arguments:
  address      The address for all operations but --clean (string)
  name         The name for name editing (string)

Optional Arguments:
  tags         The tags (string, default: "99-User-Defined")
  source       The source (string, default: "TrueBlocks")
  symbol       The symbol (string, default: "")
  decimals     The decimals (uint64, default: 0)

Options:
  --help       Show this help message and exit
  --dryrun     Simulate the command, but do not change the database
  --autoname   Given an address, query the chain for ERC20 values and update
  --delete     Delete the name entry for the given address
  --undelete   Undelete the name entry for the given address
  --remove     Remove the node for the given address
  --publish    Share any customized names you've created with the community via the Unchained Index
  --clean      Clean the names database including sorting and removing dups (if any)

Evironment Variables:
  TB_NAMEMANAGER_REGULAR  If 'true', process the regular database, otherwise custom database (default: false)
  TB_NAMEMANAGER_DRYRUN   If 'true', run the command, but do not make changes to the database (default: false)
  TB_LOGLEVEL             One of [ debug | info | warn | error ] (default: info)
  `

	if err != nil {
		fmt.Fprintf(os.Stderr, "%sError: %s%s\n", colors.Red, err.Error(), colors.Off)
	}
	fmt.Fprintf(os.Stderr, "%s", helpText)

	os.Exit(0)
}
