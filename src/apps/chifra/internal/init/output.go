// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package initPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

func RunInit(cmd *cobra.Command, args []string) error {
	opts := InitFinishParse(args)

	err := opts.ValidateInit()
	if err != nil {
		return err
	}

	// EXISTING_CODE
	err = opts.HandleInit()
	if err != nil {
		logger.Fatal(err)
	}

	return nil
	// EXISTING_CODE
}

func ServeInit(w http.ResponseWriter, r *http.Request) bool {
	opts := InitFinishParseApi(w, r)

	err := opts.ValidateInit()
	if err != nil {
		output.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// EXISTING_CODE
	err = opts.HandleInit()
	if err != nil {
		output.RespondWithError(w, http.StatusInternalServerError, err)
	}

	return true
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
