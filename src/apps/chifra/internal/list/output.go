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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
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
	monitorArray := make([]monitor.Monitor, 0, len(opts.Addrs))
	err = opts.HandleFreshenMonitors(&monitorArray)
	if err != nil {
		return err
	}

	if opts.Count {
		return opts.HandleListCount(monitorArray)
	} else {
		return opts.HandleListAppearances(monitorArray)
	}
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
	monitorArray := make([]monitor.Monitor, 0, len(opts.Addrs))
	err = opts.HandleFreshenMonitors(&monitorArray)
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}
	if opts.Count {
		err = opts.HandleListCount(monitorArray)
	} else {
		err = opts.HandleListAppearances(monitorArray)
	}
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
	}
	return true
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
