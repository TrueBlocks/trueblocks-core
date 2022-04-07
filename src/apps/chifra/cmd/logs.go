// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package cmd

// EXISTING_CODE
import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	logsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/logs"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// logsCmd represents the logs command
var logsCmd = &cobra.Command{
	Use:     usageLogs,
	Short:   shortLogs,
	Long:    longLogs,
	Version: versionText,
	RunE:    logsPkg.RunLogs,
}

var usageLogs = `logs [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)`

var shortLogs = "retrieve logs for the given transaction(s)"

var longLogs = `Purpose:
  Retrieve logs for the given transaction(s).`

var notesLogs = `
Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
  - If you specify a 32-byte hash, it will be assumed to be a transaction hash, if the transaction is not found, it will be used as a topic.`

func init() {
	logsCmd.Flags().SortFlags = false

	logsCmd.Flags().BoolVarP(&logsPkg.GetOptions().Articulate, "articulate", "a", false, "articulate the retrieved data if ABIs can be found")
	globals.InitGlobals(logsCmd, &logsPkg.GetOptions().Globals)

	logsCmd.SetUsageTemplate(UsageWithNotes(notesLogs))
	logsCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(logsCmd)
}
