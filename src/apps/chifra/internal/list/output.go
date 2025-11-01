// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package listPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunList handles the list command for the command line. Returns error only as per cobra.
func RunList(cmd *cobra.Command, args []string) error {
	_ = cmd
	opts := listFinishParse(args)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("list", &opts.Globals)
	return opts.ListInternal(rCtx)
}

// ServeList handles the list command for the API. Returns an error.
func ServeList(w http.ResponseWriter, r *http.Request) error {
	opts := listFinishParseApi(w, r)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("list", w, &opts.Globals)
	err := opts.ListInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("list", err, &opts.Globals)
	return err
}

// ListInternal handles the internal workings of the list command. Returns an error.
func (opts *ListOptions) ListInternal(rCtx *output.RenderCtx) error {
	var err error
	if err = opts.validateList(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra list"
	// EXISTING_CODE
	monitorArray := make([]monitor.Monitor, 0, len(opts.Addrs))
	if canceled, err := opts.FreshenMonitorsForList(&monitorArray); err != nil || canceled {
		return err
	}
	// EXISTING_CODE
	if opts.Count {
		err = opts.HandleCount(rCtx, monitorArray)
	} else if opts.Bounds {
		err = opts.HandleBounds(rCtx, monitorArray)
	} else {
		err = opts.HandleShow(rCtx, monitorArray)
	}
	timer.Report(msg)

	return err
}

// GetListOptions returns the options for this tool so other tools may use it.
func GetListOptions(args []string, g *globals.GlobalOptions) *ListOptions {
	ret := listFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}
