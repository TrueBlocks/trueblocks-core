// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package cmd

// EXISTING_CODE
import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	receiptsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/receipts"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// receiptsCmd represents the receipts command
var receiptsCmd = &cobra.Command{
	Use:     usageReceipts,
	Short:   shortReceipts,
	Long:    longReceipts,
	Version: versionText,
	RunE:    runWithFileSupport,
}

const usageReceipts = `receipts [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)`

const shortReceipts = "retrieve receipts for the given transaction(s)"

const longReceipts = `Purpose:
  Retrieve receipts for the given transaction(s).`

const notesReceipts = `
Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.`

func init() {
	receiptsCmd.Flags().SortFlags = false

	receiptsCmd.Flags().BoolVarP(&receiptsPkg.GetOptions().Articulate, "articulate", "a", false, "articulate the retrieved data if ABIs can be found")
	globals.InitGlobals(receiptsCmd, &receiptsPkg.GetOptions().Globals)

	receiptsCmd.SetUsageTemplate(UsageWithNotes(notesReceipts))
	receiptsCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(receiptsCmd)
}

// `runWithFileSupport` runs a command in the usual way unless `--file` is specified.
// If it is specified, this function will parse the file and then run the command
// in series of independent calls (just like calling `chifra` N times on the command line,
// but without wasting time and resources for the startup)
func runWithFileSupport(cmd *cobra.Command, args []string) error {
	// try to open the file
	filePath, err := cmd.Flags().GetString("file")
	if err != nil {
		return err
	}
	if filePath == "" {
		// `--file` has not been provided, run the command as usual
		return receiptsPkg.RunReceipts(cmd, args)
	}

	// parse commands file
	commandsFile, err := file.ParseCommandsFile(cmd, filePath)
	if err != nil {
		return err
	}

	for _, line := range commandsFile.Lines {
		receiptsPkg.ResetOptions()
		// TODO: this is nasty hack
		if receiptsPkg.GetOptions().Globals.Chain == "" {
			receiptsPkg.GetOptions().Globals.Chain = config.GetDefaultChain()
		}

		// first, parse flags from the command line
		_ = cmd.ParseFlags(os.Args[1:])
		// next, parse flags from the file
		err = cmd.ParseFlags(line.Flags)
		if err != nil {
			return fmt.Errorf("while parsing cmd flags: %s", err)
		}
		err = receiptsPkg.RunReceipts(cmd, line.Args)
		if err != nil {
			return fmt.Errorf("while running: %s", err)
		}
	}
	return nil
}
