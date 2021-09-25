package cmd

/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/

import (
	"os"
	"strconv"

	"github.com/spf13/cobra"
)

type abisOptionsType struct {
	known   bool
	sol     string
	find    string
	source  bool
	classes bool
}

var AbisOpts abisOptionsType

// abisCmd represents the abis command
var abisCmd = &cobra.Command{
	Use: `abis [flags] <address> [address...]

Arguments:
  addrs - list of one or more smart contracts whose ABI to grab from EtherScan (required)`,
	Short: "fetches the ABI for a smart contract",
	Long: `Purpose:
  Fetches the ABI for a smart contract.`,
	Run: runAbis,
}

func init() {
	abisCmd.Flags().SortFlags = false
	abisCmd.PersistentFlags().SortFlags = false
	abisCmd.SetOut(os.Stderr)

	abisCmd.Flags().BoolVarP(&AbisOpts.known, "known", "k", false, "load common 'known' ABIs from cache")
	abisCmd.Flags().StringVarP(&AbisOpts.sol, "sol", "s", "", "file name of .sol file from which to create a new known abi (without .sol)")
	abisCmd.Flags().StringVarP(&AbisOpts.find, "find", "f", "", "try to search for a function declaration given a four byte code")
	abisCmd.Flags().BoolVarP(&AbisOpts.source, "source", "o", false, "show the source of the ABI information")
	abisCmd.Flags().BoolVarP(&AbisOpts.classes, "classes", "c", false, "generate classDefinitions folder and class definitions")

	rootCmd.AddCommand(abisCmd)
}

func runAbis(cmd *cobra.Command, args []string) {
	options := ""
	if AbisOpts.known {
		options += " --known"
	}
	if len(AbisOpts.sol) > 0 {
		options += " --sol " + AbisOpts.sol
	}
	if len(AbisOpts.find) > 0 {
		options += " --find " + AbisOpts.find
	}
	if AbisOpts.source {
		options += " --source"
	}
	if AbisOpts.classes {
		options += " --classes"
	}
	for _, arg := range args {
		options += " " + arg
	}
	PassItOn("/Users/jrush/.local/bin/chifra/grabABI", options, strconv.FormatUint(0, 10))
}
