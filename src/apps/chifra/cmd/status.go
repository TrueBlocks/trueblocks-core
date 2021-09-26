package cmd

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
 * Parts of this file were generated with makeClass --gocmds.
 */

import (
	"errors"
	"fmt"
	"os"

	"github.com/spf13/cobra"
)

// statusCmd represents the status command
var statusCmd = &cobra.Command{
	Use:   usageStatus,
	Short: shortStatus,
	Long:  longStatus,
	Run:   runStatus,
	Args:  ValidatePositionals(validateStatusArgs),
}

var usageStatus = `status [flags] [mode...]

Arguments:
  modes - the type of status info to retrieve`

var shortStatus = "report on the status of the TrueBlocks system"

var longStatus = `Purpose:
  Report on the status of the TrueBlocks system.`

type statusOptionsType struct {
	details    bool
	types      string
	depth      uint64
	report     bool
	terse      bool
	migrate    string
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
	statusCmd.Flags().StringVarP(&StatusOpts.types, "types", "t", "", "for caches mode only, which type(s) of cache to report")
	statusCmd.Flags().Uint64VarP(&StatusOpts.depth, "depth", "p", 0, "for cache mode only, number of levels deep to report (hidden)")
	statusCmd.Flags().BoolVarP(&StatusOpts.report, "report", "r", false, "show a summary of the current status of TrueBlocks (deprecated) (hidden)")
	statusCmd.Flags().BoolVarP(&StatusOpts.terse, "terse", "e", false, "show a terse summary report (hidden)")
	statusCmd.Flags().StringVarP(&StatusOpts.migrate, "migrate", "m", "", "either effectuate or test to see if a migration is necessary (hidden)")
	statusCmd.Flags().BoolVarP(&StatusOpts.get_config, "get_config", "g", false, "returns JSON data of the editable configuration file items (hidden)")
	statusCmd.Flags().BoolVarP(&StatusOpts.set_config, "set_config", "s", false, "accepts JSON in an env variable and writes it to configuration files (hidden)")
	statusCmd.Flags().Uint64VarP(&StatusOpts.test_start, "test_start", "S", 0, "first block to process (inclusive -- testing only) (hidden)")
	statusCmd.Flags().Uint64VarP(&StatusOpts.test_end, "test_end", "E", 0, "last block to process (inclusive -- testing only) (hidden)")
	statusCmd.Flags().SortFlags = false
	statusCmd.PersistentFlags().SortFlags = false

	PostNotes = ""
	rootCmd.AddCommand(statusCmd)
}

func runStatus(cmd *cobra.Command, args []string) {
	options := ""
	if StatusOpts.details {
		options += " --details"
	}
	if len(StatusOpts.types) > 0 {
		options += " --types " + StatusOpts.types
	}
	if StatusOpts.depth > 0 {
		options += " --depth " + fmt.Sprintf("%d", StatusOpts.depth)
	}
	if StatusOpts.report {
		options += " --report"
	}
	if StatusOpts.terse {
		options += " --terse"
	}
	if len(StatusOpts.migrate) > 0 {
		options += " --migrate " + StatusOpts.migrate
	}
	if StatusOpts.get_config {
		options += " --get_config"
	}
	if StatusOpts.set_config {
		options += " --set_config"
	}
	if StatusOpts.test_start > 0 {
		options += " --test_start " + fmt.Sprintf("%d", StatusOpts.test_start)
	}
	if StatusOpts.test_end > 0 {
		options += " --test_end " + fmt.Sprintf("%d", StatusOpts.test_end)
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("/Users/jrush/.local/bin/chifra/cacheStatus", options, arguments)
}

func validateStatusArgs(cmd *cobra.Command, args []string) error {
	if len(args) > 0 && args[0] == "12" {
		return ErrFunc(cmd, errors.New("Invalid argument "+args[0]))
	}
	return nil
}
