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
/*
 * Parts of this file were generated with makeClass --gocmds. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

import (
	// EXISTING_CODE
	"os"

	"github.com/spf13/cobra"
	// EXISTING_CODE
)

// slurpCmd represents the slurp command
var slurpCmd = &cobra.Command{
	Use:   usageSlurp,
	Short: shortSlurp,
	Long:  longSlurp,
	Run:   runSlurp,
	Args:  validateSlurpArgs,
}

var usageSlurp = `slurp [flags] <address> [address...] [block...]

Arguments:
  addrs - one or more addresses to slurp from Etherscan (required)
  blocks - an optional range of blocks to slurp`

var shortSlurp = "fetch data from EtherScan for any address"

var longSlurp = `Purpose:
  Fetch data from EtherScan for any address.`

var notesSlurp = `
Notes:
  - Portions of this software are Powered by Etherscan.io APIs.`

type slurpOptionsType struct {
	types       []string
	appearances bool
}

var SlurpOpts slurpOptionsType

func init() {
	slurpCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE
	slurpCmd.Flags().SortFlags = false
	slurpCmd.PersistentFlags().SortFlags = false
	slurpCmd.Flags().StringSliceVarP(&SlurpOpts.types, "types", "t", nil, `which types of transactions to request
One or more of ext, int, token, nfts, miner, uncles, all`)
	slurpCmd.Flags().BoolVarP(&SlurpOpts.appearances, "appearances", "p", false, "show only the blocknumer.tx_id appearances of the exported transactions")
	slurpCmd.Flags().SortFlags = false
	slurpCmd.PersistentFlags().SortFlags = false
	// EXISTING_CODE
	// EXISTING_CODE

	slurpCmd.SetUsageTemplate(HelpWithNotes(notesSlurp))
	rootCmd.AddCommand(slurpCmd)
}

func runSlurp(cmd *cobra.Command, args []string) {
	options := ""
	for _, t := range SlurpOpts.types {
		options += " --types " + t
	}
	if SlurpOpts.appearances {
		options += " --appearances"
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	// EXISTING_CODE
	// EXISTING_CODE
	PassItOn(GetCommandPath("ethslurp"), options, arguments)
}

// EXISTING_CODE
// EXISTING_CODE

func validateSlurpArgs(cmd *cobra.Command, args []string) error {
	var err error
	// EXISTING_CODE
	err = validateOneAddr(args)
	if err != nil {
		return err
	}
	err = validateEnumSlice("--types", SlurpOpts.types, "[ext|int|token|nfts|miner|uncles|all]")
	if err != nil {
		return err
	}
	// EXISTING_CODE
	// validate global arguments
	err = validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}
	return nil
}
