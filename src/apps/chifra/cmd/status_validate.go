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
	"strconv"

	"github.com/spf13/cobra"
)

func validateStatusArgs(cmd *cobra.Command, args []string) error {
	if StatusOpts.depth > 3 {
		return makeError("--depth parameter ({0}) must be less than four (4)", strconv.FormatUint(StatusOpts.depth, 10))
	}

	err := validateEnumSlice("--types", StatusOpts.types, "[blocks|txs|traces|slurps|prices|all]")
	if err != nil {
		return err
	}

	err = validateEnumSlice("--migrate", StatusOpts.migrate, "[test|abi_cache|block_cache|tx_cache|trace_cache|recon_cache|name_cache|slurp_cache|all]")
	if err != nil {
		return err
	}

	err = validateEnumSlice("modes", args, "[index|monitors|collections|names|abis|caches|some|all]")
	if err != nil {
		return err
	}

	err = validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}

	return nil
}
