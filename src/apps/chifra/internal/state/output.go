// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package statePkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunState handles the state command for the command line. Returns error only as per cobra.
func RunState(cmd *cobra.Command, args []string) (err error) {
	opts := stateFinishParse(args)
	outputHelpers.SetEnabledForCmds("state", opts.IsPorted())
	outputHelpers.SetWriterForCommand("state", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.StateInternal()
	return
}

// ServeState handles the state command for the API. Returns error and a bool if handled
func ServeState(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := stateFinishParseApi(w, r)
	outputHelpers.SetEnabledForCmds("state", opts.IsPorted())
	outputHelpers.InitJsonWriterApi("state", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.StateInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("state", err, &opts.Globals)
	return
}

// StateInternal handles the internal workings of the state command.  Returns error and a bool if handled
func (opts *StateOptions) StateInternal() (err error, handled bool) {
	err = opts.validateState()
	if err != nil {
		return err, true
	}

	timer := logger.NewTimer()
	msg := "chifra state"
	// EXISTING_CODE
	if !opts.IsPorted() {
		logger.Fatal("Should not happen in StateInternal")
	}

	handled = true
	if opts.Globals.Decache {
		err = opts.HandleDecache()
	} else if opts.Call != "" {
		err = opts.HandleCall()
	} else {
		err = opts.HandleShow()
	}
	// EXISTING_CODE
	timer.Report(msg)

	return
}

// GetStateOptions returns the options for this tool so other tools may use it.
func GetStateOptions(args []string, g *globals.GlobalOptions) *StateOptions {
	ret := stateFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *StateOptions) IsPorted() (ported bool) {
	// EXISTING_CODE
	ported = true
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
