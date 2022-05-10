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

// RunList handles the list command for the command line. Returns error only as per cobra.
func RunList(cmd *cobra.Command, args []string) (err error) {
	opts := ListFinishParse(args)
	// JINKY
	// JINKY
	err, _ = opts.ListInternal()
	return
}

// ServeList handles the list command for the API. Returns error and a bool if handled
func ServeList(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := ListFinishParseApi(w, r)
	// JINKY
	// JINKY
	return opts.ListInternal()
}

// ListInternal handles the internal workings of the list command.  Returns error and a bool if handled
func (opts *ListOptions) ListInternal() (err error, handled bool) {
	err = opts.ValidateList()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	handled = true // everything is handled even on failure

	// We always freshen the monitors. This call fills the monitors array.
	monitorArray := make([]monitor.Monitor, 0, len(opts.Addrs))
	err = opts.HandleFreshenMonitors(&monitorArray)
	if err != nil {
		return
	}

	if opts.Count {
		err = opts.HandleListCount(monitorArray)
		if err != nil {
			return
		}
	} else if !opts.Silent {
		err = opts.HandleListAppearances(monitorArray)
		if err != nil {
			return
		}
	}
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE
