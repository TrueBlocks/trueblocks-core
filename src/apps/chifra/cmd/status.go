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
 * Parts of this file were generated with makeClass --gocmds. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
package cmd

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/utils"
	"github.com/spf13/cobra"
)

// statusCmd represents the status command
var statusCmd = &cobra.Command{
	Use:   usageStatus,
	Short: shortStatus,
	Long:  longStatus,
	Run:   runStatus,
	Args:  validateStatusArgs,
}

var usageStatus = `status [flags] [mode...]

Arguments:
  modes - the type of status info to retrieve
	One or more of index, monitors, collections, names, abis, caches, some, all`

var shortStatus = "report on the status of the TrueBlocks system"

var longStatus = `Purpose:
  Report on the status of the TrueBlocks system.`

var notesStatus = ``

type statusOptionsType struct {
	details    bool
	types      []string
	depth      uint64
	report     bool
	terse      bool
	migrate    []string
	get_config bool
	set_config bool
	test_start uint64
	test_end   uint64
}

var StatusOpts statusOptionsType

func init() {
	statusCmd.SetOut(os.Stderr)

	statusCmd.Flags().SortFlags = false
	statusCmd.PersistentFlags().SortFlags = false
	statusCmd.Flags().BoolVarP(&StatusOpts.details, "details", "d", false, "include details about items found in monitors, slurps, abis, or price caches")
	statusCmd.Flags().StringSliceVarP(&StatusOpts.types, "types", "t", nil, `for caches mode only, which type(s) of cache to report
One or more of blocks, txs, traces, slurps, prices, all`)
	statusCmd.Flags().Uint64VarP(&StatusOpts.depth, "depth", "p", 0, "for cache mode only, number of levels deep to report (hidden)")
	statusCmd.Flags().BoolVarP(&StatusOpts.report, "report", "r", false, "run the command with no options for the same result (hidden)")
	statusCmd.Flags().BoolVarP(&StatusOpts.terse, "terse", "e", false, "show a terse summary report (hidden)")
	statusCmd.Flags().StringSliceVarP(&StatusOpts.migrate, "migrate", "m", nil, `either effectuate or test to see if a migration is necessary (hidden)
One or more of test, abi_cache, block_cache, tx_cache, trace_cache, recon_cache, name_cache, slurp_cache, all`)
	statusCmd.Flags().BoolVarP(&StatusOpts.get_config, "get_config", "g", false, "returns JSON data of the editable configuration file items (hidden)")
	statusCmd.Flags().BoolVarP(&StatusOpts.set_config, "set_config", "s", false, "accepts JSON in an env variable and writes it to configuration files (hidden)")
	statusCmd.Flags().Uint64VarP(&StatusOpts.test_start, "test_start", "S", 0, "first block to process (inclusive -- testing only) (hidden)")
	statusCmd.Flags().Uint64VarP(&StatusOpts.test_end, "test_end", "E", 0, "last block to process (inclusive -- testing only) (hidden)")
	if utils.IsTestMode() {
		statusCmd.Flags().MarkHidden("depth")
		statusCmd.Flags().MarkHidden("report")
		statusCmd.Flags().MarkHidden("terse")
		statusCmd.Flags().MarkHidden("migrate")
		statusCmd.Flags().MarkHidden("get_config")
		statusCmd.Flags().MarkHidden("set_config")
		statusCmd.Flags().MarkHidden("test_start")
		statusCmd.Flags().MarkHidden("test_end")
	}
	statusCmd.Flags().SortFlags = false
	statusCmd.PersistentFlags().SortFlags = false

	statusCmd.SetUsageTemplate(UsageWithNotes(notesStatus))
	rootCmd.AddCommand(statusCmd)
}

// EXISTING_CODE
// EXISTING_CODE
