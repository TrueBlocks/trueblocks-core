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

const usageStatus = `status <mode> [mode...] [flags]

Arguments:
  modes - the (optional) name of the binary cache to report on, terse otherwise
	One or more of [ abis | blocks | monitors | names | recons | slurps | tmp | traces | txs | blooms | index | ripe | staging | unripe | maps | some | all ]`

const shortStatus = "report on the state of the internal binary caches"

const longStatus = `Purpose:
  Report on the state of the internal binary caches.`

const notesStatus = `
Notes:
  - The some mode includes index, monitors, names, slurps, and abis.
  - If no mode is supplied, a terse report is generated.
  - This tool ignores the --format option, all results other than terse are json.`

func init() {
	statusCmd.Flags().SortFlags = false

	globals.InitGlobals(statusCmd, &statusPkg.GetOptions().Globals)

	statusCmd.SetUsageTemplate(UsageWithNotes(notesStatus))
	statusCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(statusCmd)
}
