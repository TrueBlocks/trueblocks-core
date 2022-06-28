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
	initPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/init"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// initCmd represents the init command
var initCmd = &cobra.Command{
	Use:     usageInit,
	Short:   shortInit,
	Long:    longInit,
	Version: versionText,
	RunE:    initPkg.RunInit,
}

const usageInit = `init [flags]`

const shortInit = "initialize the TrueBlocks system by downloading from IPFS"

const longInit = `Purpose:
  Initialize the TrueBlocks system by downloading from IPFS.`

const notesInit = `
Notes:
  - Re-run chifra init as often as you wish. It will repair or freshen the index.`

func init() {
	initCmd.Flags().SortFlags = false

	initCmd.Flags().BoolVarP(&initPkg.GetOptions().All, "all", "a", false, "in addition to Bloom filters, download full index chunks")
	globals.InitGlobals(initCmd, &initPkg.GetOptions().Globals)

	initCmd.SetUsageTemplate(UsageWithNotes(notesInit))
	initCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(initCmd)
}
