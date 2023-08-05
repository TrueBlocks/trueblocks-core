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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunStatus handles the status command for the command line. Returns error only as per cobra.
func RunStatus(cmd *cobra.Command, args []string) (err error) {
	opts := statusFinishParse(args)
	outputHelpers.SetEnabledForCmds("status", opts.IsPorted())
	outputHelpers.SetWriterForCommand("status", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.StatusInternal()
	return
}

// ServeStatus handles the status command for the API. Returns error and a bool if handled
func ServeStatus(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := statusFinishParseApi(w, r)
	outputHelpers.SetEnabledForCmds("status", opts.IsPorted())
	outputHelpers.InitJsonWriterApi("status", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.StatusInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("status", err, &opts.Globals)
	return
}

// StatusInternal handles the internal workings of the status command.  Returns error and a bool if handled
func (opts *StatusOptions) StatusInternal() (err error, handled bool) {
	err = opts.validateStatus()
	if err != nil {
		return err, true
	}

	timer := logger.NewTimer()
	msg := "chifra status"
	// EXISTING_CODE
	if !opts.IsPorted() {
		logger.Fatal("Should not happen in NamesInternal")
	}

	handled = true
	if len(opts.ModeTypes) > 0 {
		err = opts.HandleShow()
	} else {
		err = opts.HandleStatusTerse()
	}
	// EXISTING_CODE
	timer.Report(msg)

	return
}

// GetStatusOptions returns the options for this tool so other tools may use it.
func GetStatusOptions(args []string, g *globals.GlobalOptions) *StatusOptions {
	ret := statusFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *StatusOptions) IsPorted() (ported bool) {
	// EXISTING_CODE
	ported = true
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
