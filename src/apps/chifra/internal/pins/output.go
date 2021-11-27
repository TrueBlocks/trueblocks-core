// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package pinsPkg

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

func ServePins(w http.ResponseWriter, r *http.Request) bool {
	opts := FromRequest(w, r)

	err := opts.ValidatePins()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// EXISTING_CODE
	err = pinlib.EstablishIndexFolders()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	if opts.List {
		err := opts.ListInternal()
		if err != nil {
			opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
			return true
		}
	}

	if opts.Init {
		err := opts.InitInternal()
		if err != nil {
			opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
			return true
		}
	}

	return true
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
