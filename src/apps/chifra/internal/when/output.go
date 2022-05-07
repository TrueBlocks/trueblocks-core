// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package whenPkg

// EXISTING_CODE
import (
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

func RunWhen(cmd *cobra.Command, args []string) error {
	opts := WhenFinishParse(args)

	err := opts.ValidateWhen()
	if err != nil {
		return err
	}

	// EXISTING_CODE
	if opts.List {
		err := opts.HandleWhenList()
		if err != nil {
			return err
		}
		if len(opts.Blocks) == 0 {
			return nil
		}
		// continue but don't show headers
		opts.List = false
		opts.Globals.NoHeader = true
	}

	return opts.Globals.PassItOn("whenBlock", opts.Globals.Chain, opts.ToCmdLine(), opts.Globals.ToCmdLine())
	// EXISTING_CODE
}

func ServeWhen(w http.ResponseWriter, r *http.Request) bool {
	opts := FromRequest(w, r)

	err := opts.ValidateWhen()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// EXISTING_CODE
	if opts.List {
		err = opts.HandleWhenList()
		if err != nil {
			logger.Fatal("Cannot generate when list", err)
			return false
		}
		if len(opts.Blocks) == 0 {
			return true
		}
		// continue but don't show headers or --list
		r.URL.RawQuery = strings.Replace(r.URL.RawQuery, "list", "noop", -1)
		r.URL.RawQuery += "&no_header"
	}
	// return opts.Globals.PassItOn("whenBlock", opts.Globals.Chain, opts.ToCmdLine(), opts.Globals.ToCmdLine())
	return false
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
