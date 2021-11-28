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
	whenPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/when"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// whenCmd represents the when command
var whenCmd = &cobra.Command{
	Use:     usageWhen,
	Short:   shortWhen,
	Long:    longWhen,
	Version: "GHC-TrueBlocks//0.17.0-alpha",
	RunE:    whenPkg.RunWhen,
}

var usageWhen = `when [flags] < block | date > [ block... | date... ]

Arguments:
  blocks - one or more dates, block numbers, hashes, or special named blocks (see notes) (required)`

var shortWhen = "find block(s) based on date, blockNum, timestamp, or 'special'"

var longWhen = `Purpose:
  Find block(s) based on date, blockNum, timestamp, or 'special'.`

var notesWhen = `
Notes:
  - The block list may contain any combination of number, hash, date, special named blocks.
  - Dates must be formatted in JSON format: YYYY-MM-DD[THH[:MM[:SS]]].`

func init() {
	whenCmd.Flags().SortFlags = false

	whenCmd.Flags().BoolVarP(&whenPkg.Options.List, "list", "l", false, "export a list of the 'special' blocks")
	whenCmd.Flags().BoolVarP(&whenPkg.Options.Timestamps, "timestamps", "t", false, "ignore other options and generate timestamps only")
	whenCmd.Flags().BoolVarP(&whenPkg.Options.Check, "check", "c", false, "available only with --timestamps, checks the validity of the timestamp data (hidden)")
	whenCmd.Flags().BoolVarP(&whenPkg.Options.Fix, "fix", "f", false, "available only with --timestamps, fixes incorrect timestamps if any (hidden)")
	whenCmd.Flags().BoolVarP(&whenPkg.Options.Count, "count", "u", false, "available only with --timestamps, returns the number of timestamps in the cache (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		whenCmd.Flags().MarkHidden("check")
		whenCmd.Flags().MarkHidden("fix")
		whenCmd.Flags().MarkHidden("count")
	}
	globals.InitGlobals(whenCmd, &whenPkg.Options.Globals)

	whenCmd.SetUsageTemplate(UsageWithNotes(notesWhen))
	whenCmd.SetOut(os.Stderr)

	chifraCmd.AddCommand(whenCmd)
}
