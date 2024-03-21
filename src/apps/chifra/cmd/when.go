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
	whenPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/when"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// whenCmd represents the when command
var whenCmd = &cobra.Command{
	Use:     usageWhen,
	Short:   shortWhen,
	Long:    longWhen,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("when", func() *globals.GlobalOptions {
		return &whenPkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("when", whenPkg.RunWhen, whenPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &whenPkg.GetOptions().Globals
	}),
}

const usageWhen = `when [flags] < block | date > [ block... | date... ]

Arguments:
  blocks - one or more dates, block numbers, hashes, or special named blocks (see notes)`

const shortWhen = "find block(s) based on date, blockNum, timestamp, or 'special'"

const longWhen = `Purpose:
  Find block(s) based on date, blockNum, timestamp, or 'special'.`

const notesWhen = `
Notes:
  - The block list may contain any combination of number, hash, date, special named blocks.
  - Block numbers, timestamps, or dates in the future are estimated with 13 second blocks.
  - Dates must be formatted in JSON format: YYYY-MM-DD[THH[:MM[:SS]]].`

func init() {
	var capabilities caps.Capability // capabilities for chifra when
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Caching)

	whenCmd.Flags().SortFlags = false

	whenCmd.Flags().BoolVarP(&whenPkg.GetOptions().List, "list", "l", false, "export a list of the 'special' blocks")
	whenCmd.Flags().BoolVarP(&whenPkg.GetOptions().Timestamps, "timestamps", "t", false, "display or process timestamps")
	whenCmd.Flags().BoolVarP(&whenPkg.GetOptions().Count, "count", "U", false, "with --timestamps only, returns the number of timestamps in the cache")
	whenCmd.Flags().Uint64VarP(&whenPkg.GetOptions().Truncate, "truncate", "n", 0, "with --timestamps only, truncates the timestamp file at this block (hidden)")
	whenCmd.Flags().BoolVarP(&whenPkg.GetOptions().Repair, "repair", "r", false, "with --timestamps only, repairs block(s) in the block range by re-querying from the chain")
	whenCmd.Flags().BoolVarP(&whenPkg.GetOptions().Check, "check", "c", false, "with --timestamps only, checks the validity of the timestamp data")
	whenCmd.Flags().BoolVarP(&whenPkg.GetOptions().Update, "update", "u", false, "with --timestamps only, bring the timestamp database forward to the latest block")
	whenCmd.Flags().BoolVarP(&whenPkg.GetOptions().Deep, "deep", "d", false, "with --timestamps --check only, verifies timestamps from on chain (slow)")
	if os.Getenv("TEST_MODE") != "true" {
		_ = whenCmd.Flags().MarkHidden("truncate")
	}
	globals.InitGlobals("when", whenCmd, &whenPkg.GetOptions().Globals, capabilities)

	whenCmd.SetUsageTemplate(UsageWithNotes(notesWhen))
	whenCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(whenCmd)
}
