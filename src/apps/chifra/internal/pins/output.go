package pinsPkg

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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

// EXISTING_CODE
import (
	"fmt"
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib"
	"github.com/spf13/cobra"
)

var errCustomFolderMissing = `Attempt to create customized indexPath (%s) failed.
Please create the folder or adjust the setting by editing $CONFIG/trueBlocks.toml.
`

// EXISTING_CODE

func RunPins(cmd *cobra.Command, args []string) error {
	opts := PinsFinishParse(args)

	err := opts.ValidatePins()
	if err != nil {
		return err
	}

	// EXISTING_CODE
	err = pinlib.EstablishIndexFolders()
	if err != nil {
		if err1, ok := err.(*pinlib.ErrCustomizedPath); ok {
			fmt.Printf(errCustomFolderMissing, err1.GetIndexPath())
			return nil
		}
		logger.Fatal(err)
	}

	if opts.List {
		err := opts.ListInternal()
		if err != nil {
			logger.Fatal("Cannot open local manifest file", err)
		}
		return nil
	}

	if opts.Init {
		err := opts.InitInternal()
		if err != nil {
			logger.Fatal(err)
		}
		return nil
	}

	if opts.Sleep != .25 {
		logger.Fatal("Not implemented")
	}

	if opts.Share {
		logger.Fatal("Not implemented")
	}

	return nil
	// EXISTING_CODE
}

func ServePins(w http.ResponseWriter, r *http.Request) {
	opts := FromRequest(w, r)

	err := opts.ValidatePins()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return
	}

	// EXISTING_CODE
	err = pinlib.EstablishIndexFolders()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return
	}

	if opts.List {
		err := opts.ListInternal()
		if err != nil {
			opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
			return
		}
	}

	if opts.Init {
		err := opts.InitInternal()
		if err != nil {
			opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		}
	}
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
