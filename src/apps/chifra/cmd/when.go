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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/internal/globals"
	whenPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/internal/when"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// whenCmd represents the when command
var whenCmd = &cobra.Command{
	Use:     usageWhen,
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

	whenCmd.Flags().BoolVarP(&whenPkg.GetOptions().List, "list", "l", false, `export a list of the 'special' blocks`)
	whenCmd.Flags().BoolVarP(&whenPkg.GetOptions().Timestamps, "timestamps", "t", false, `display or process timestamps`)
	whenCmd.Flags().BoolVarP(&whenPkg.GetOptions().Count, "count", "U", false, `with --timestamps only, returns the number of timestamps in the cache`)
	whenCmd.Flags().Uint64VarP((*uint64)(&whenPkg.GetOptions().Truncate), "truncate", "n", 0, `with --timestamps only, truncates the timestamp file at this block (hidden)`)
	whenCmd.Flags().BoolVarP(&whenPkg.GetOptions().Repair, "repair", "r", false, `with --timestamps only, repairs block(s) in the block range by re-querying from the chain`)
	whenCmd.Flags().BoolVarP(&whenPkg.GetOptions().Check, "check", "c", false, `with --timestamps only, checks the validity of the timestamp data`)
	whenCmd.Flags().BoolVarP(&whenPkg.GetOptions().Update, "update", "u", false, `with --timestamps only, bring the timestamp database forward to the latest block`)
	whenCmd.Flags().Uint64VarP(&whenPkg.GetOptions().Deep, "deep", "d", 0, `with --timestamps --check only, verifies every N timestamp directly from the chain (slow)`)
	if !base.IsTestMode() {
		_ = whenCmd.Flags().MarkHidden("truncate")
	}
	globals.InitGlobals("when", whenCmd, &whenPkg.GetOptions().Globals, capabilities)

	whenCmd.SetUsageTemplate(UsageWithNotes(notesWhen))
	whenCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(whenCmd)
}
