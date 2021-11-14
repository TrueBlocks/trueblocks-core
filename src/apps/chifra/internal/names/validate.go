package namesPkg

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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

func Validate(cmd *cobra.Command, args []string) error {
	if Options.Tags && anyBase() {
		return validate.Usage("The {0} option is not available{1}.", "--tags", " with any other option")
	}

	if Options.Collections && anyBase() {
		return validate.Usage("The {0} option is not available{1}.", "--collection", " with any other option")
	}

	Options.TestLog()

	return globals.ValidateGlobals(&Options.Globals, args)
}

func anyBase() bool {
	return Options.Expand ||
		Options.MatchCase ||
		Options.All ||
		Options.Custom ||
		Options.Prefund ||
		Options.Named ||
		Options.Addr ||
		Options.ToCustom ||
		Options.Clean
}
