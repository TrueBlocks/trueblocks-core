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
	"fmt"
	"os"
	"os/exec"
	"strconv"

	"github.com/spf13/cobra"
	"github.com/spf13/pflag"
)

type abiOptionsType struct {
	known   bool
	sol     string
	find    string
	source  bool
	classes bool
}

var abiOpts abiOptionsType

// abisCmd represents the abis command
var abisCmd = &cobra.Command{
	Use: `abis [flags] <address> [address...]

Arguments:
  addrs - list of one or more smart contracts whose ABI to grab from EtherScan (required)`,
	Short: "fetches the ABI for a smart contract",
	Long: `Purpose:
  Fetches the ABI for a smart contract.`,
	PreRun: func(cmd *cobra.Command, args []string) {
	},
	Run: runAbi,
}

func init() {
	abisCmd.Flags().SortFlags = false
	abisCmd.PersistentFlags().SortFlags = false
	abisCmd.SetOut(os.Stderr)

	abisCmd.Flags().BoolVarP(&abiOpts.known, "known", "k", false, "load common 'known' ABIs from cache")
	abisCmd.Flags().StringVarP(&abiOpts.sol, "sol", "s", "", "file name of .sol file from which to create a new known abi (without .sol)")
	abisCmd.Flags().StringVarP(&abiOpts.find, "find", "f", "", "try to search for a function declaration given a four byte code")
	abisCmd.Flags().BoolVarP(&abiOpts.source, "source", "o", false, "show the source of the ABI information")
	abisCmd.Flags().BoolVarP(&abiOpts.classes, "classes", "c", false, "generate classDefinitions folder and class definitions")

	rootCmd.AddCommand(abisCmd)
}

func PassItOn(path string, options string) {
	what := exec.Command(path, options)
	what.Env = append(append(os.Environ(), "API_MODE=true"), "GO_HELP=true")
	out, _ := what.Output()
	output := string(out[:])
	fmt.Printf("%s", output)
}

func fn(f *pflag.Flag) {
	fmt.Printf("%v\n", f)
}

func runAbi(cmd *cobra.Command, args []string) {
	fs := cmd.Flags()
	fs.VisitAll(fn)

	options := ""
	for _, option := range args {
		options += " " + option
	}
	if abiOpts.known {
		options += " --known"
	}
	if abiOpts.source {
		options += " --source"
	}
	if abiOpts.classes {
		options += " --classes"
	}
	if len(abiOpts.sol) > 0 {
		options += " --sol " + abiOpts.sol
	}
	if len(abiOpts.find) > 0 {
		options += " --find " + abiOpts.find
	}
	if len(RootOpts.fmt) > 0 {
		options += " --fmt " + RootOpts.fmt
	}
	if RootOpts.verbose > 0 {
		options += " --verbose " + strconv.FormatUint(uint64(RootOpts.verbose), 10)
	}
	path := "/Users/jrush/.local/bin/chifra/grabABI"
	PassItOn(path, options)
}
