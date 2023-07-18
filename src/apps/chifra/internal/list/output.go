// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
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
func RunList(cmd *cobra.Command, args []string) (err error) {
	opts := listFinishParse(args)
	outputHelpers.SetEnabledForCmds("list", opts.IsPorted())
	outputHelpers.SetWriterForCommand("list", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.ListInternal()
	return
}

// ServeList handles the list command for the API. Returns error and a bool if handled
func ServeList(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := listFinishParseApi(w, r)
	outputHelpers.SetEnabledForCmds("list", opts.IsPorted())
	outputHelpers.InitJsonWriterApi("list", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.ListInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("list", err, &opts.Globals)
	return
}

// ListInternal handles the internal workings of the list command.  Returns error and a bool if handled
func (opts *ListOptions) ListInternal() (err error, handled bool) {
	err = opts.validateList()
	if err != nil {
		return err, true
	}

	timer := logger.NewTimer()
	msg := "chifra list"
	// EXISTING_CODE
	handled = true // everything is handled even on failure

	// We always freshen the monitors. This call fills the monitors array.
	monitorArray := make([]monitor.Monitor, 0, len(opts.Addrs))
	if canceled, err := opts.HandleFreshenMonitors(&monitorArray); err != nil || canceled {
		return err, true
	}

	if opts.Count {
		err = opts.HandleCount(monitorArray)
	} else if opts.Bounds {
		err = opts.HandleBounds(monitorArray)
	} else if !opts.Silent {
		err = opts.HandleListAppearances(monitorArray)
	}
	// EXISTING_CODE
	timer.Report(msg)

	return
}

// GetListOptions returns the options for this tool so other tools may use it.
func GetListOptions(args []string, g *globals.GlobalOptions) *ListOptions {
	ret := listFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *ListOptions) IsPorted() (ported bool) {
	// EXISTING_CODE
	ported = true
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
