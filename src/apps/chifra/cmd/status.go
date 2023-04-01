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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// statusCmd represents the status command
var statusCmd = &cobra.Command{
	Use:     usageStatus,
	Short:   shortStatus,
	Long:    longStatus,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("status", func() *globals.GlobalOptions {
		return &statusPkg.GetOptions().Globals
	}),
	RunE:    file.RunWithFileSupport("status", statusPkg.RunStatus, statusPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &statusPkg.GetOptions().Globals
	}),
}

const usageStatus = `status [flags] <mode>
Arguments:
  mode - the name of the binary cache to report on
	One of [ index | monitors | names | abis | caches | some | all ]`

const shortStatus = "report on the state of the internal binary caches"

const longStatus = `Purpose:
  Report on the state of the internal binary caches.`

const notesStatus = ``

func init() {
	statusCmd.Flags().SortFlags = false

	statusCmd.Flags().StringSliceVarP(&statusPkg.GetOptions().Types, "types", "t", nil, `for caches mode only, which type(s) of cache to report
One or more of [ blocks | txs | traces | slurps | all ]`)
	globals.InitGlobals(statusCmd, &statusPkg.GetOptions().Globals)

	statusCmd.SetUsageTemplate(UsageWithNotes(notesStatus))
	statusCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(statusCmd)
}
