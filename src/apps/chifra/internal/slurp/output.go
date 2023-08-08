// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package slurpPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunSlurp handles the slurp command for the command line. Returns error only as per cobra.
func RunSlurp(cmd *cobra.Command, args []string) (err error) {
	opts := slurpFinishParse(args)
	outputHelpers.SetEnabledForCmds("slurp", opts.IsPorted())
	outputHelpers.SetWriterForCommand("slurp", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.SlurpInternal()
	return
}

// ServeSlurp handles the slurp command for the API. Returns error and a bool if handled
func ServeSlurp(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := slurpFinishParseApi(w, r)
	outputHelpers.SetEnabledForCmds("slurp", opts.IsPorted())
	outputHelpers.InitJsonWriterApi("slurp", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.SlurpInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("slurp", err, &opts.Globals)
	return
}

// SlurpInternal handles the internal workings of the slurp command.  Returns error and a bool if handled
func (opts *SlurpOptions) SlurpInternal() (err error, handled bool) {
	err = opts.validateSlurp()
	if err != nil {
		return err, true
	}

	timer := logger.NewTimer()
	msg := "chifra slurp"
	// EXISTING_CODE
	if !opts.IsPorted() {
		logger.Fatal("Should not happen.")
	}

	handled = true
	if opts.Globals.Decache {
		err = opts.HandleDecache()
	} else if opts.Appearances {
		err = opts.HandleAppearances()
	} else {
		err = opts.HandleShow()
	}
	// EXISTING_CODE
	timer.Report(msg)

	return
}

// GetSlurpOptions returns the options for this tool so other tools may use it.
func GetSlurpOptions(args []string, g *globals.GlobalOptions) *SlurpOptions {
	ret := slurpFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *SlurpOptions) IsPorted() (ported bool) {
	// EXISTING_CODE
	ported = true
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
