// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package statePkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunState handles the state command for the command line. Returns error only as per cobra.
func RunState(cmd *cobra.Command, args []string) error {
	opts := stateFinishParse(args)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("state", &opts.Globals)
	return opts.StateInternal(rCtx)
}

// ServeState handles the state command for the API. Returns an error.
func ServeState(w http.ResponseWriter, r *http.Request) error {
	opts := stateFinishParseApi(w, r)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("state", w, &opts.Globals)
	err := opts.StateInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("state", err, &opts.Globals)
	return err
}

// StateInternal handles the internal workings of the state command. Returns an error.
func (opts *StateOptions) StateInternal(rCtx *output.RenderCtx) error {
	var err error
	if err = opts.validateState(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra state"
	// EXISTING_CODE
	// EXISTING_CODE
	if opts.Globals.Decache {
		err = opts.HandleDecache(rCtx)
	} else if opts.Call {
		err = opts.HandleCall(rCtx)
	} else if opts.Send {
		err = opts.HandleSend(rCtx)
	} else {
		err = opts.HandleShow(rCtx)
	}
	timer.Report(msg)

	return err
}

// GetStateOptions returns the options for this tool so other tools may use it.
func GetStateOptions(args []string, g *globals.GlobalOptions) *StateOptions {
	ret := stateFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}
