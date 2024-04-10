// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package cmd

// EXISTING_CODE
import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	logsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/logs"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// logsCmd represents the logs command
var logsCmd = &cobra.Command{
	Use:     usageLogs,
	Long:    longLogs,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("logs", func() *globals.GlobalOptions {
		return &logsPkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("logs", logsPkg.RunLogs, logsPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &logsPkg.GetOptions().Globals
	}),
}

const usageLogs = `logs [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)`

const longLogs = `Purpose:
  Retrieve logs for the given transaction(s).`

const notesLogs = `
Notes:
  - The transactions list may be one or more transaction hashes, blockNumber.transactionID pairs, or a blockHash.transactionID pairs.
  - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
  - If the queried node does not store historical state, the results for most older transactions are undefined.
  - If you specify a 32-byte hash, it will be assumed to be a transaction hash, if it is not, the hash will be used as a topic.`

func init() {
	var capabilities caps.Capability // capabilities for chifra logs
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Caching)
	capabilities = capabilities.Add(caps.Raw)

	logsCmd.Flags().SortFlags = false

	logsCmd.Flags().StringSliceVarP(&logsPkg.GetOptions().Emitter, "emitter", "m", nil, `filter logs to show only those logs emitted by the given address(es)`)
	logsCmd.Flags().StringSliceVarP(&logsPkg.GetOptions().Topic, "topic", "B", nil, `filter logs to show only those with this topic(s)`)
	logsCmd.Flags().BoolVarP(&logsPkg.GetOptions().Articulate, "articulate", "a", false, `articulate the retrieved data if ABIs can be found`)
	globals.InitGlobals("logs", logsCmd, &logsPkg.GetOptions().Globals, capabilities)

	logsCmd.SetUsageTemplate(UsageWithNotes(notesLogs))
	logsCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(logsCmd)
}
