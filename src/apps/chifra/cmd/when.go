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
	Version: versionText,
	RunE:    whenPkg.RunWhen,
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
  - Dates must be formatted in JSON format: YYYY-MM-DD[THH[:MM[:SS]]].`

func init() {
	whenCmd.Flags().SortFlags = false

	whenCmd.Flags().BoolVarP(&whenPkg.GetOptions().List, "list", "l", false, "export a list of the 'special' blocks")
	whenCmd.Flags().BoolVarP(&whenPkg.GetOptions().Timestamps, "timestamps", "t", false, "ignore other options and generate timestamps only")
	whenCmd.Flags().BoolVarP(&whenPkg.GetOptions().Check, "check", "c", false, "available only with --timestamps, checks the validity of the timestamp data (hidden)")
	whenCmd.Flags().BoolVarP(&whenPkg.GetOptions().Count, "count", "u", false, "available only with --timestamps, returns the number of timestamps in the cache (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		whenCmd.Flags().MarkHidden("check")
		whenCmd.Flags().MarkHidden("count")
	}
	globals.InitGlobals(whenCmd, &whenPkg.GetOptions().Globals)

	whenCmd.SetUsageTemplate(UsageWithNotes(notesWhen))
	whenCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(whenCmd)
}
