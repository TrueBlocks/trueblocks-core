// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package tracesPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunTraces handles the traces command for the command line. Returns error only as per cobra.
func RunTraces(cmd *cobra.Command, args []string) (err error) {
	opts := tracesFinishParse(args)
	outputHelpers.SetEnabledForCmds("traces", opts.IsPorted())
	outputHelpers.SetWriterForCommand("traces", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.TracesInternal()
	return
}

// ServeTraces handles the traces command for the API. Returns error and a bool if handled
func ServeTraces(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := tracesFinishParseApi(w, r)
	outputHelpers.SetEnabledForCmds("traces", opts.IsPorted())
	outputHelpers.InitJsonWriterApi("traces", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.TracesInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("traces", err, &opts.Globals)
	return
}

// TracesInternal handles the internal workings of the traces command.  Returns error and a bool if handled
func (opts *TracesOptions) TracesInternal() (err error, handled bool) {
	err = opts.validateTraces()
	if err != nil {
		return err, true
	}

	timer := logger.NewTimer()
	msg := "chifra traces"
	// EXISTING_CODE
	if !opts.IsPorted() {
		logger.Fatal("Should never happen")
	}

	handled = true
	if opts.Globals.Decache {
		err = opts.HandleDecache()
	} else if opts.Count {
		err = opts.HandleCounts()
	} else if len(opts.Filter) > 0 {
		err = opts.HandleFilter()
	} else {
		err = opts.HandleShow()
	}
	// EXISTING_CODE
	timer.Report(msg)

	return
}

// GetTracesOptions returns the options for this tool so other tools may use it.
func GetTracesOptions(args []string, g *globals.GlobalOptions) *TracesOptions {
	ret := tracesFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *TracesOptions) IsPorted() (ported bool) {
	// EXISTING_CODE
	ported = true
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
