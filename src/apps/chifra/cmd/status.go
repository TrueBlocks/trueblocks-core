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
	statusPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/status"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// statusCmd represents the status command
var statusCmd = &cobra.Command{
	Use:     usageStatus,
	Short:   shortStatus,
	Long:    longStatus,
	Version: versionText,
	RunE:    statusPkg.RunStatus,
}

var usageStatus = `status [flags] [mode...]

Arguments:
  modes - the type of status info to retrieve
	One or more of [ index | monitors | collections | names | abis | caches | some | all ]`

var shortStatus = "report on the status of the TrueBlocks system"

var longStatus = `Purpose:
  Report on the status of the TrueBlocks system.`

var notesStatus = ``

func init() {
	statusCmd.Flags().SortFlags = false

	statusCmd.Flags().BoolVarP(&statusPkg.GetOptions().Details, "details", "d", false, "include details about items found in monitors, slurps, abis, or price caches")
	statusCmd.Flags().StringSliceVarP(&statusPkg.GetOptions().Types, "types", "t", nil, `for caches mode only, which type(s) of cache to report
One or more of [ blocks | txs | traces | slurps | prices | all ]`)
	statusCmd.Flags().Uint64VarP(&statusPkg.GetOptions().Depth, "depth", "p", 0, "for cache mode only, number of levels deep to report (hidden)")
	statusCmd.Flags().BoolVarP(&statusPkg.GetOptions().Report, "report", "r", false, "run the command with no options for the same result (hidden)")
	statusCmd.Flags().BoolVarP(&statusPkg.GetOptions().Terse, "terse", "e", false, "show a terse summary report (hidden)")
	statusCmd.Flags().StringVarP(&statusPkg.GetOptions().Migrate, "migrate", "m", "", `either effectuate or test to see if a migration is necessary (hidden)
One of [ test | all ]`)
	statusCmd.Flags().Uint64VarP(&statusPkg.GetOptions().FirstBlock, "first_block", "F", 0, "first block to process (inclusive -- testing only) (hidden)")
	statusCmd.Flags().Uint64VarP(&statusPkg.GetOptions().LastBlock, "last_block", "L", 0, "last block to process (inclusive -- testing only) (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		statusCmd.Flags().MarkHidden("depth")
		statusCmd.Flags().MarkHidden("report")
		statusCmd.Flags().MarkHidden("terse")
		statusCmd.Flags().MarkHidden("migrate")
		statusCmd.Flags().MarkHidden("first_block")
		statusCmd.Flags().MarkHidden("last_block")
	}
	globals.InitGlobals(statusCmd, &statusPkg.GetOptions().Globals)

	statusCmd.SetUsageTemplate(UsageWithNotes(notesStatus))
	statusCmd.SetOut(os.Stderr)

	chifraCmd.AddCommand(statusCmd)
}
