// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package statusPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunStatus handles the status command for the command line. Returns error only as per cobra.
func RunStatus(cmd *cobra.Command, args []string) (err error) {
	opts := statusFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.StatusInternal()
	return
}

// ServeStatus handles the status command for the API. Returns error and a bool if handled
func ServeStatus(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := statusFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE
	return opts.StatusInternal()
}

// StatusInternal handles the internal workings of the status command.  Returns error and a bool if handled
func (opts *StatusOptions) StatusInternal() (err error, handled bool) {
	err = opts.validateStatus()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	if len(opts.Migrate) > 0 {
		for _, m := range opts.Modes {
			if m == "index" {
				if opts.Migrate == "test" {
					// Note this does not return if a migration is needed
					index.CheckBackLevelIndex(opts.Globals.Chain)
				}
			}
		}
	}

	if opts.Globals.ApiMode {
		return nil, false
	}

	handled = true
	err = opts.Globals.PassItOn("cacheStatus", opts.Globals.Chain, opts.toCmdLine(), opts.getEnvStr())
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE
