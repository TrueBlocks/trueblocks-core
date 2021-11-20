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
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

// EXISTING_CODE
import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	slurpPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/slurp"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// slurpCmd represents the slurp command
var slurpCmd = &cobra.Command{
	Use:     usageSlurp,
	Short:   shortSlurp,
	Long:    longSlurp,
	Version: "GHC-TrueBlocks//0.16.1-alpha",
	RunE:    slurpPkg.RunSlurp,
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

func init() {
	slurpCmd.Flags().SortFlags = false

	slurpCmd.Flags().StringSliceVarP(&slurpPkg.Options.Types, "types", "t", nil, `which types of transactions to request
One or more of [ ext | int | token | nfts | miner | uncles | all ]`)
	slurpCmd.Flags().BoolVarP(&slurpPkg.Options.Appearances, "appearances", "p", false, "show only the blocknumer.tx_id appearances of the exported transactions")
	globals.InitGlobals(slurpCmd, &slurpPkg.Options.Globals)

	slurpCmd.SetUsageTemplate(UsageWithNotes(notesSlurp))
	slurpCmd.SetOut(os.Stderr)

	chifraCmd.AddCommand(slurpCmd)
}
