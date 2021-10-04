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
package cmd

import (
	"os"

	"github.com/spf13/cobra"
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

	slurpCmd.Flags().SortFlags = false
	slurpCmd.PersistentFlags().SortFlags = false
	slurpCmd.Flags().StringSliceVarP(&SlurpOpts.types, "types", "t", nil, `which types of transactions to request
One or more of ext, int, token, nfts, miner, uncles, all`)
	slurpCmd.Flags().BoolVarP(&SlurpOpts.appearances, "appearances", "p", false, "show only the blocknumer.tx_id appearances of the exported transactions")
	slurpCmd.Flags().SortFlags = false
	slurpCmd.PersistentFlags().SortFlags = false

	slurpCmd.SetUsageTemplate(UsageWithNotes(notesSlurp))
	rootCmd.AddCommand(slurpCmd)
}

// EXISTING_CODE
// EXISTING_CODE
