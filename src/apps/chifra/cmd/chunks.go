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

	chunksPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/chunks"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// chunksCmd represents the chunks command
var chunksCmd = &cobra.Command{
	Use:     usageChunks,
	Short:   shortChunks,
	Long:    longChunks,
	Version: "GHC-TrueBlocks//0.18.0-alpha",
	RunE:    chunksPkg.RunChunks,
}

var usageChunks = `chunks [flags] <block> [block...]

Arguments:
  blocks - an optional list of blocks to process`

var shortChunks = "manage and investigate chunks and bloom filters"

var longChunks = `Purpose:
  Manage and investigate chunks and bloom filters.`

var notesChunks = `
Notes:
  - Only a single block in a given chunk needs to be supplied.`

func init() {
	chunksCmd.Flags().SortFlags = false

	chunksCmd.Flags().BoolVarP(&chunksPkg.Options.Check, "check", "c", false, "check the validity of the chunk or bloom")
	chunksCmd.Flags().StringVarP(&chunksPkg.Options.Extract, "extract", "e", "", `show some or all of the contents of the chunk or bloom filters
One of [ header | addr_table | app_table | chunks | blooms ]`)
	chunksCmd.Flags().BoolVarP(&chunksPkg.Options.Stats, "stats", "s", false, "for the --list option only, display statistics about each chunk or bloom")
	chunksCmd.Flags().BoolVarP(&chunksPkg.Options.Save, "save", "a", false, "for the --extract option only, save the entire chunk to a similarly named file as well as display")
	globals.InitGlobals(chunksCmd, &chunksPkg.Options.Globals)

	chunksCmd.SetUsageTemplate(UsageWithNotes(notesChunks))
	chunksCmd.SetOut(os.Stderr)

	chifraCmd.AddCommand(chunksCmd)
}
