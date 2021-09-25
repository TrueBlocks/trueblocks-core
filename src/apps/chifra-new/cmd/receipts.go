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

type receiptsOptionsType struct {
	articulate bool
}

var ReceiptsOpts receiptsOptionsType

// receiptsCmd represents the receipts command
var receiptsCmd = &cobra.Command{
	Use: `receipts [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)`,
	Short: "retrieve receipts for the given transaction(s)",
	Long: `Purpose:
  Retrieve receipts for the given transaction(s).`,
	Run: runReceipts,
}

func init() {
	receiptsCmd.Flags().SortFlags = false
	receiptsCmd.PersistentFlags().SortFlags = false
	receiptsCmd.SetOut(os.Stderr)

	receiptsCmd.Flags().BoolVarP(&ReceiptsOpts.articulate, "articulate", "a", false, "articulate the retrieved data if ABIs can be found")

	rootCmd.AddCommand(receiptsCmd)
}

func runReceipts(cmd *cobra.Command, args []string) {
	options := ""
	if ReceiptsOpts.articulate {
		options += " --articulate"
	}
	for _, arg := range args {
		options += " " + arg
	}
	PassItOn("/Users/jrush/.local/bin/chifra/getReceipts", options, strconv.FormatUint(0, 10))
}
