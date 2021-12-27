// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package listPkg

// EXISTING_CODE
import (
	"net/http"

	exportPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/export"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

func RunList(cmd *cobra.Command, args []string) error {
	opts := ListFinishParse(args)

	err := opts.ValidateList()
	if err != nil {
		return err
	}

	// EXISTING_CODE
	exportPkg.Options.Appearances = true
	if opts.Count {
		exportPkg.Options.Count = true
	}
	if opts.FirstBlock > 0 {
		exportPkg.Options.FirstBlock = opts.FirstBlock
	}
	if opts.LastBlock > 0 {
		exportPkg.Options.LastBlock = opts.LastBlock
	}
	exportPkg.Options.Globals = opts.Globals

	return exportPkg.RunExport(cmd, args)
	// EXISTING_CODE
}

func ServeList(w http.ResponseWriter, r *http.Request) bool {
	opts := FromRequest(w, r)

	err := opts.ValidateList()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// EXISTING_CODE
	return false
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
