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
	listPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/list"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// listCmd represents the list command
var listCmd = &cobra.Command{
	Use:     usageList,
	Short:   shortList,
	Long:    longList,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("list", func() *globals.GlobalOptions {
		return &listPkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("list", listPkg.RunList, listPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &listPkg.GetOptions().Globals
	}),
}

const usageList = `list [flags] <address> [address...]

Arguments:
  addrs - one or more addresses (0x...) to list (required)`

const shortList = "list every appearance of an address anywhere on the chain"

const longList = `Purpose:
  List every appearance of an address anywhere on the chain.`

const notesList = `
Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - No other options are permitted when --silent is selected.`

func init() {
	var capabilities caps.Capability // capabilities for chifra list
	capabilities = capabilities.Add(caps.Default)

	listCmd.Flags().SortFlags = false

	listCmd.Flags().BoolVarP(&listPkg.GetOptions().Count, "count", "U", false, "display only the count of records for each monitor")
	listCmd.Flags().BoolVarP(&listPkg.GetOptions().NoZero, "no_zero", "z", false, "for the --count option only, suppress the display of zero appearance accounts")
	listCmd.Flags().BoolVarP(&listPkg.GetOptions().Bounds, "bounds", "b", false, "report first and last block this address appears")
	listCmd.Flags().BoolVarP(&listPkg.GetOptions().Unripe, "unripe", "u", false, "list transactions labeled upripe (i.e. less than 28 blocks old)")
	listCmd.Flags().BoolVarP(&listPkg.GetOptions().Silent, "silent", "s", false, "freshen the monitor only (no reporting)")
	listCmd.Flags().Uint64VarP(&listPkg.GetOptions().FirstRecord, "first_record", "c", 0, "the first record to process")
	listCmd.Flags().Uint64VarP(&listPkg.GetOptions().MaxRecords, "max_records", "e", 250, "the maximum number of records to process")
	listCmd.Flags().BoolVarP(&listPkg.GetOptions().Reversed, "reversed", "E", false, "produce results in reverse chronological order")
	listCmd.Flags().StringVarP(&listPkg.GetOptions().Publisher, "publisher", "P", "", "for some query options, the publisher of the index (hidden)")
	listCmd.Flags().Uint64VarP(&listPkg.GetOptions().FirstBlock, "first_block", "F", 0, "first block to export (inclusive, ignored when freshening)")
	listCmd.Flags().Uint64VarP(&listPkg.GetOptions().LastBlock, "last_block", "L", 0, "last block to export (inclusive, ignored when freshening)")
	if os.Getenv("TEST_MODE") != "true" {
		_ = listCmd.Flags().MarkHidden("publisher")
	}
	globals.InitGlobals("list", listCmd, &listPkg.GetOptions().Globals, capabilities)

	listCmd.SetUsageTemplate(UsageWithNotes(notesList))
	listCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(listCmd)
}
