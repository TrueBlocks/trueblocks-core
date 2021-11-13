package pins

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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

var errCustomFolderMissing = `Attempt to create customized indexPath (%s) failed.
Please create the folder or adjust the setting by editing $CONFIG/trueBlocks.toml.
`

func Run(cmd *cobra.Command, args []string) error {
	// This only happens in API mode when there's been an error. Here, we print the error
	if len(validate.Errors) > 0 {
		output.PrintJson(&output.JsonFormatted{})
		return nil
	}

	err := pinlib.EstablishIndexFolders()
	if err != nil {
		if err, ok := err.(*pinlib.ErrCustomizedPath); ok {
			fmt.Printf(errCustomFolderMissing, err.GetIndexPath())
			return nil
		}
		logger.Fatal(err)
	}

	if Options.List {
		PrintManifestHeader()
		HandleList(&Options)
		return nil
	}

	if Options.Init {
		PrintManifestHeader()
		HandleInit(&Options)
		return nil
	}

	if Options.Sleep != .25 {
		logger.Fatal("Not implemented")
	}

	if Options.Share {
		logger.Fatal("Not implemented")
	}
	return nil
}
