// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package whenPkg

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

// RunWhen handles the when command for the command line. Returns error only as per cobra.
func RunWhen(cmd *cobra.Command, args []string) error {
	_ = cmd
	opts := whenFinishParse(args)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("when", &opts.Globals)
	return opts.WhenInternal(rCtx)
}

// ServeWhen handles the when command for the API. Returns an error.
func ServeWhen(w http.ResponseWriter, r *http.Request) error {
	opts := whenFinishParseApi(w, r)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("when", w, &opts.Globals)
	err := opts.WhenInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("when", err, &opts.Globals)
	return err
}

// WhenInternal handles the internal workings of the when command. Returns an error.
func (opts *WhenOptions) WhenInternal(rCtx *output.RenderCtx) error {
	var err error
	if err = opts.validateWhen(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra when"
	// EXISTING_CODE
	// EXISTING_CODE
	if opts.Globals.Decache {
		err = opts.HandleDecache(rCtx)
	} else if opts.List {
		err = opts.HandleList(rCtx)
	} else if opts.Timestamps {
		err = opts.HandleTimestamps(rCtx)
	} else {
		err = opts.HandleShow(rCtx)
	}
	timer.Report(msg)

	return err
}

// GetWhenOptions returns the options for this tool so other tools may use it.
func GetWhenOptions(args []string, g *globals.GlobalOptions) *WhenOptions {
	ret := whenFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}
