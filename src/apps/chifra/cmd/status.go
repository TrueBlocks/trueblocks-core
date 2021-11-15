/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * The file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */
package cmd

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/globals"
	statusPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/status"
	"github.com/spf13/cobra"
)

// statusCmd represents the status command
var statusCmd = &cobra.Command{
	Use:   usageStatus,
	Short: shortStatus,
	Long:  longStatus,
	RunE:  statusPkg.RunStatus,
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

	statusCmd.Flags().BoolVarP(&statusPkg.Options.Details, "details", "d", false, "include details about items found in monitors, slurps, abis, or price caches")
	statusCmd.Flags().StringSliceVarP(&statusPkg.Options.Types, "types", "t", nil, `for caches mode only, which type(s) of cache to report
One or more of [ blocks | txs | traces | slurps | prices | all ]`)
	statusCmd.Flags().Uint64VarP(&statusPkg.Options.Depth, "depth", "p", 0, "for cache mode only, number of levels deep to report (hidden)")
	statusCmd.Flags().BoolVarP(&statusPkg.Options.Report, "report", "r", false, "run the command with no options for the same result (hidden)")
	statusCmd.Flags().BoolVarP(&statusPkg.Options.Terse, "terse", "e", false, "show a terse summary report (hidden)")
	statusCmd.Flags().StringSliceVarP(&statusPkg.Options.Migrate, "migrate", "m", nil, `either effectuate or test to see if a migration is necessary (hidden)
One or more of [ test | abi_cache | block_cache | tx_cache | trace_cache | recon_cache | name_cache | slurp_cache | all ]`)
	statusCmd.Flags().BoolVarP(&statusPkg.Options.GetConfig, "get_config", "g", false, "returns JSON data of the editable configuration file items (hidden)")
	statusCmd.Flags().BoolVarP(&statusPkg.Options.SetConfig, "set_config", "s", false, "accepts JSON in an env variable and writes it to configuration files (hidden)")
	statusCmd.Flags().Uint64VarP(&statusPkg.Options.TestStart, "test_start", "S", 0, "first block to process (inclusive -- testing only) (hidden)")
	statusCmd.Flags().Uint64VarP(&statusPkg.Options.TestEnd, "test_end", "E", 0, "last block to process (inclusive -- testing only) (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		statusCmd.Flags().MarkHidden("depth")
		statusCmd.Flags().MarkHidden("report")
		statusCmd.Flags().MarkHidden("terse")
		statusCmd.Flags().MarkHidden("migrate")
		statusCmd.Flags().MarkHidden("get_config")
		statusCmd.Flags().MarkHidden("set_config")
		statusCmd.Flags().MarkHidden("test_start")
		statusCmd.Flags().MarkHidden("test_end")
	}
	globals.InitGlobals(statusCmd, &statusPkg.Options.Globals)

	statusCmd.SetUsageTemplate(UsageWithNotes(notesStatus))
	statusCmd.SetOut(os.Stderr)

	chifraCmd.AddCommand(statusCmd)
}
