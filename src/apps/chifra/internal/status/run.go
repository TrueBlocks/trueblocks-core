package status

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
import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/root"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/spf13/cobra"
)

func Run(cmd *cobra.Command, args []string) error {
	options := ""
	if Options.Details {
		options += " --details"
	}
	for _, t := range Options.Types {
		options += " --types " + t
	}
	if Options.Depth != utils.NOPOS {
		options += " --depth " + fmt.Sprintf("%d", Options.Depth)
	}
	if Options.Terse {
		options += " --terse"
	}
	for _, t := range Options.Migrate {
		options += " --migrate " + t
	}
	if Options.GetConfig {
		options += " --get_config"
	}
	if Options.SetConfig {
		options += " --set_config"
	}
	if Options.TestStart > 0 {
		if utils.IsTestMode() {
			options += " --test_start " + fmt.Sprintf("%d", Options.TestStart)
		}
	}
	if Options.TestEnd != utils.NOPOS {
		if utils.IsTestMode() {
			options += " --test_end " + fmt.Sprintf("%d", Options.TestEnd)
		}
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	root.PassItOn("cacheStatus", options, arguments)
	return nil
}
