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
	statusPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/status"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// statusCmd represents the status command
var statusCmd = &cobra.Command{
	Use:     usageStatus,
	Long:    longStatus,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("status", func() *globals.GlobalOptions {
		return &statusPkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("status", statusPkg.RunStatus, statusPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &statusPkg.GetOptions().Globals
	}),
}

const usageStatus = `status <mode> [mode...] [flags]

Arguments:
  modes - the (optional) name of the binary cache to report on, terse otherwise
    One or more of [ index | blooms | blocks | transactions | traces | logs | statements | results | state | tokens | monitors | names | abis | slurps | staging | unripe | maps | some | all ]`

const longStatus = `Purpose:
  Report on the state of the internal binary caches.`

const notesStatus = `
Notes:
  - The some mode includes index, monitors, names, slurps, and abis.
  - If no mode is supplied, a terse report is generated.
  - The --chains option may be used alone to return only chain configuration information.
  - The --caches option may be used alone to return only cache information.
  - Using both --chains and --caches together returns both types of information in a single status object.
  - Both --chains and --caches may be used with modes and --diagnose/--healthcheck for complete customization.`

func init() {
	var capabilities caps.Capability // capabilities for chifra status
	capabilities = capabilities.Add(caps.Default)

	statusCmd.Flags().SortFlags = false

	statusCmd.Flags().BoolVarP(&statusPkg.GetOptions().Diagnose, "diagnose", "d", false, `same as the default but with additional diagnostics`)
	statusCmd.Flags().Uint64VarP(&statusPkg.GetOptions().FirstRecord, "first_record", "c", 0, `the first record to process`)
	statusCmd.Flags().Uint64VarP(&statusPkg.GetOptions().MaxRecords, "max_records", "e", 10000, `the maximum number of records to process`)
	statusCmd.Flags().BoolVarP(&statusPkg.GetOptions().Chains, "chains", "a", false, `include a list of chain configurations in the output`)
	statusCmd.Flags().BoolVarP(&statusPkg.GetOptions().Caches, "caches", "s", false, `include a list of cache items in the output`)
	statusCmd.Flags().BoolVarP(&statusPkg.GetOptions().Healthcheck, "healthcheck", "k", false, `an alias for the diagnose endpoint`)
	globals.InitGlobals("status", statusCmd, &statusPkg.GetOptions().Globals, capabilities)

	statusCmd.SetUsageTemplate(UsageWithNotes(notesStatus))
	statusCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(statusCmd)
}
