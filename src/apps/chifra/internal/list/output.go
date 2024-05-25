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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunList handles the list command for the command line. Returns error only as per cobra.
func RunList(cmd *cobra.Command, args []string) error {
	opts := listFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("list", &opts.Globals)
	return opts.ListInternal()
}

// ServeList handles the list command for the API. Returns an error.
func ServeList(w http.ResponseWriter, r *http.Request) error {
	opts := listFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("list", w, &opts.Globals)
	err := opts.ListInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("list", err, &opts.Globals)
	return err
}

// ListInternal handles the internal workings of the list command. Returns an error.
func (opts *ListOptions) ListInternal() error {
	var err error
	if err = opts.validateList(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra list"
	// EXISTING_CODE
	// We always freshen the monitors. This call fills the monitors array.
	monitorArray := make([]monitor.Monitor, 0, len(opts.Addrs))
	var updater = monitor.NewUpdater(opts.Globals.Chain, opts.Globals.TestMode, opts.Addrs)
	updater.PublisherAddr = opts.PublisherAddr
	if canceled, err := updater.FreshenMonitors(&monitorArray); err != nil || canceled {
		return err
	}

	if opts.Count {
		err = opts.HandleCount(monitorArray)
	} else if opts.Bounds {
		err = opts.HandleBounds(monitorArray)
	} else if !opts.Silent {
		err = opts.HandleShow(monitorArray)
	}
	// EXISTING_CODE
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

// EXISTING_CODE
// EXISTING_CODE
