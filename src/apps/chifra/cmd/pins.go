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
	pinsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/pins"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// pinsCmd represents the pins command
var pinsCmd = &cobra.Command{
	Use:     usagePins,
	Short:   shortPins,
	Long:    longPins,
	Version: versionText,
	RunE:    pinsPkg.RunPins,
}

var usagePins = `pins [flags]`

var shortPins = "manage pinned index of appearances and associated blooms"

var longPins = `Purpose:
  Manage pinned index of appearances and associated blooms.`

var notesPins = `
Notes:
  - One of --list or --init is required.
  - Re-run chifra init as often as you wish. It will repair or freshen the index.
  - The --share option works only if an IPFS daemon is running.`

func init() {
	pinsCmd.Flags().SortFlags = false

	pinsCmd.Flags().BoolVarP(&pinsPkg.GetOptions().List, "list", "l", false, "list the bloom and index hashes from local cache or IPFS")
	pinsCmd.Flags().BoolVarP(&pinsPkg.GetOptions().Init, "init", "i", false, "download the blooms or index chunks from IPFS")
	pinsCmd.Flags().BoolVarP(&pinsPkg.GetOptions().All, "all", "a", false, "in addition to Bloom filters, download full index chunks")
	pinsCmd.Flags().BoolVarP(&pinsPkg.GetOptions().Share, "share", "S", false, "share downloaded data by pinning it to IPFS (the IPFS daemon must be running)")
	pinsCmd.Flags().Float64VarP(&pinsPkg.GetOptions().Sleep, "sleep", "s", .25, "throttle requests by this many seconds")
	pinsCmd.Flags().BoolVarP(&pinsPkg.GetOptions().Freshen, "freshen", "f", false, "check for new bloom or index chunks and download if available (hidden)")
	pinsCmd.Flags().BoolVarP(&pinsPkg.GetOptions().Remote, "remote", "r", false, "for --list mode only, recover the manifest from IPFS via UnchainedIndex smart contract (hidden)")
	pinsCmd.Flags().BoolVarP(&pinsPkg.GetOptions().InitAll, "init_all", "n", false, "use --init --all instead (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		pinsCmd.Flags().MarkHidden("freshen")
		pinsCmd.Flags().MarkHidden("remote")
		pinsCmd.Flags().MarkHidden("init_all")
	}
	globals.InitGlobals(pinsCmd, &pinsPkg.GetOptions().Globals)

	pinsCmd.SetUsageTemplate(UsageWithNotes(notesPins))
	pinsCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(pinsCmd)
}
