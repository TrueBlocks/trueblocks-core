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
package cmd

import (
	"fmt"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/utils"
	"github.com/spf13/cobra"
)

func validateStatusArgs(cmd *cobra.Command, args []string) error {
	if StatusOpts.depth > 3 {
		return validate.Usage("--depth parameter ({0}) must be less than four (4)", strconv.FormatUint(StatusOpts.depth, 10))
	}

	err := validate.ValidateEnumSlice("--types", StatusOpts.types, "[blocks|txs|traces|slurps|prices|all]")
	if err != nil {
		return err
	}

	err = validate.ValidateEnumSlice("--migrate", StatusOpts.migrate, "[test|abi_cache|block_cache|tx_cache|trace_cache|recon_cache|name_cache|slurp_cache|all]")
	if err != nil {
		return err
	}

	err = validate.ValidateEnumSlice("modes", args, "[index|monitors|collections|names|abis|caches|some|all]")
	if err != nil {
		return err
	}

	err = validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}

	return nil
}

func runStatus(cmd *cobra.Command, args []string) {
	options := ""
	if StatusOpts.details {
		options += " --details"
	}
	for _, t := range StatusOpts.types {
		options += " --types " + t
	}
	if StatusOpts.depth != utils.NOPOS {
		options += " --depth " + fmt.Sprintf("%d", StatusOpts.depth)
	}
	if StatusOpts.terse {
		options += " --terse"
	}
	for _, t := range StatusOpts.migrate {
		options += " --migrate " + t
	}
	if StatusOpts.get_config {
		options += " --get_config"
	}
	if StatusOpts.set_config {
		options += " --set_config"
	}
	if StatusOpts.test_start > 0 {
		if utils.IsTestMode() {
			options += " --test_start " + fmt.Sprintf("%d", StatusOpts.test_start)
		}
	}
	if StatusOpts.test_end != utils.NOPOS {
		if utils.IsTestMode() {
			options += " --test_end " + fmt.Sprintf("%d", StatusOpts.test_end)
		}
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("cacheStatus", options, arguments)
}
