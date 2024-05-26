// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
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
func RunTraces(cmd *cobra.Command, args []string) error {
	opts := tracesFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("traces", &opts.Globals)
	return opts.TracesInternal()
}

// ServeTraces handles the traces command for the API. Returns an error.
func ServeTraces(w http.ResponseWriter, r *http.Request) error {
	opts := tracesFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("traces", w, &opts.Globals)
	err := opts.TracesInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("traces", err, &opts.Globals)
	return err
}

// TracesInternal handles the internal workings of the traces command. Returns an error.
func (opts *TracesOptions) TracesInternal() error {
	var err error
	if err = opts.validateTraces(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra traces"
	// EXISTING_CODE
	// EXISTING_CODE
	if opts.Globals.Decache {
		err = opts.HandleDecache()
	} else if opts.Count {
		err = opts.HandleCount()
	} else if len(opts.Filter) > 0 {
		err = opts.HandleFilter()
	} else {
		err = opts.HandleShow()
	}
	timer.Report(msg)

	return err
}

// GetTracesOptions returns the options for this tool so other tools may use it.
func GetTracesOptions(args []string, g *globals.GlobalOptions) *TracesOptions {
	ret := tracesFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}
