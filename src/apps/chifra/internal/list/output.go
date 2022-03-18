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
	"os"

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
	if opts.Newone {
		// TODO: BOGUS -- WHAT?
		// if opts.Count {
		// 	err := opts.Freshen()
		// 	if err != nil {
		// 		logger.Fatal("Could not handle count request", err)
		// 	}
		// 	err = opts.HandleListCount()
		// 	if err != nil {
		// 		logger.Fatal("Could not handle count request", err)
		// 	}
		// 	return nil
		// }
		opts.FreshenMonitor(true, os.Stdout)
		return nil
	}

	exportPkg.GetOptions().Appearances = true
	if opts.Count {
		exportPkg.GetOptions().Count = true
	}
	if opts.FirstBlock > 0 {
		exportPkg.GetOptions().FirstBlock = opts.FirstBlock
	}
	if opts.LastBlock > 0 {
		exportPkg.GetOptions().LastBlock = opts.LastBlock
	}
	exportPkg.GetOptions().Globals = opts.Globals

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
	if opts.Newone {
		// TODO: BOGUS -- WHAT?
		// if opts.Count {
		// 	err := opts.FreshenMonitor()
		// 	if err != nil {
		// 		logger.Fatal("Could not handle count request", err)
		// 	}
		// 	err = opts.HandleListCount()
		// 	if err != nil {
		// 		logger.Fatal("Could not handle count request", err)
		// 	}
		// 	return nil
		// }
	}
	return false
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
