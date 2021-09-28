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
	"errors"
	"os"

	"github.com/spf13/cobra"
	// EXISTING_CODE
)

// receiptsCmd represents the receipts command
var receiptsCmd = &cobra.Command{
	Use:   usageReceipts,
	Short: shortReceipts,
	Long:  longReceipts,
	Run:   runReceipts,
	Args:  validateReceiptsArgs,
}

var usageReceipts = `receipts [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)`

var shortReceipts = "retrieve receipts for the given transaction(s)"

var longReceipts = `Purpose:
  Retrieve receipts for the given transaction(s).`

var notesReceipts = `
Notes:
  - The transactions list may be one or more space-separated identifiers which are either a transaction hash,
    a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.`

type receiptsOptionsType struct {
	articulate bool
}

var ReceiptsOpts receiptsOptionsType

func init() {
	receiptsCmd.SetOut(os.Stderr)

	receiptsCmd.Flags().SortFlags = false
	receiptsCmd.PersistentFlags().SortFlags = false
	receiptsCmd.Flags().BoolVarP(&ReceiptsOpts.articulate, "articulate", "a", false, "articulate the retrieved data if ABIs can be found")
	receiptsCmd.Flags().SortFlags = false
	receiptsCmd.PersistentFlags().SortFlags = false

	receiptsCmd.SetUsageTemplate(HelpWithNotes(notesReceipts))
	rootCmd.AddCommand(receiptsCmd)
}

func runReceipts(cmd *cobra.Command, args []string) {
	options := ""
	if ReceiptsOpts.articulate {
		options += " --articulate"
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	// EXISTING_CODE
	// EXISTING_CODE
	PassItOn(GetCommandPath("getReceipts"), options, arguments)
}

// EXISTING_CODE
// EXISTING_CODE

func validateReceiptsArgs(cmd *cobra.Command, args []string) error {
	var err error
	// EXISTING_CODE
	if len(args) == 0 {
		return errors.New(fmtError("You must provide at least one valid transaction identifier"))
	}
	// for _, arg := range args {
	// 	valid, err := validateTxIdentifier(arg)
	// 	if !valid || err != nil {
	// 		return err
	// 	}
	// }
	// EXISTING_CODE
	err = validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}
	return nil
}
