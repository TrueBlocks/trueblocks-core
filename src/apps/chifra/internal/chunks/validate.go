package chunksPkg

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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ChunksOptions) ValidateChunks() error {
	if !Options.List && !Options.Check && len(Options.Extract) == 0 {
		return validate.Usage("Please choose at least one of {0}.", "--list, --extract, or --check")
	}

	if (Options.List && Options.Check) || (Options.List && len(Options.Extract) > 0) {
		return validate.Usage("Please choose only one of {0}.", "--list, --extract, or --check")
	}

	if Options.Stats && !Options.List {
		return validate.Usage("The {0} option is available only with {1}.", "--stats", "--list")
	}

	err := validate.ValidateEnum("--extract", Options.Extract, "[header|addr_table|app_table|chunks|blooms]")
	if err != nil {
		return err
	}

	Options.TestLog()

	return globals.ValidateGlobals(&Options.Globals)
}
