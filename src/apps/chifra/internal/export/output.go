// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package exportPkg

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

// RunExport handles the export command for the command line. Returns error only as per cobra.
func RunExport(cmd *cobra.Command, args []string) error {
	opts := exportFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("export", &opts.Globals)
	return opts.ExportInternal()
}

// ServeExport handles the export command for the API. Returns an error.
func ServeExport(w http.ResponseWriter, r *http.Request) error {
	opts := exportFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("export", w, &opts.Globals)
	err := opts.ExportInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("export", err, &opts.Globals)
	return err
}

// ExportInternal handles the internal workings of the export command. Returns an error.
func (opts *ExportOptions) ExportInternal() error {
	var err error
	if err = opts.validateExport(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra export"
	// EXISTING_CODE
	monitorArray := make([]monitor.Monitor, 0, len(opts.Addrs))
	if canceled, err := opts.FreshenMonitorsForExport(&monitorArray); err != nil || canceled {
		return err
	}
	// EXISTING_CODE
	if opts.Globals.Decache {
		err = opts.HandleDecache(monitorArray)
	} else if opts.Count {
		err = opts.HandleCount(monitorArray)
	} else if opts.Receipts {
		err = opts.HandleReceipts(monitorArray)
	} else if opts.Logs {
		err = opts.HandleLogs(monitorArray)
	} else if opts.Traces {
		err = opts.HandleTraces(monitorArray)
	} else if opts.Withdrawals {
		err = opts.HandleWithdrawals(monitorArray)
	} else if opts.Appearances {
		err = opts.HandleAppearances(monitorArray)
	} else if opts.Balances {
		err = opts.HandleBalances(monitorArray)
	} else if opts.Neighbors {
		err = opts.HandleNeighbors(monitorArray)
	} else if opts.Statements {
		err = opts.HandleStatements(monitorArray)
	} else if opts.Accounting {
		err = opts.HandleAccounting(monitorArray)
	} else {
		err = opts.HandleShow(monitorArray)
	}
	timer.Report(msg)

	return err
}

// GetExportOptions returns the options for this tool so other tools may use it.
func GetExportOptions(args []string, g *globals.GlobalOptions) *ExportOptions {
	ret := exportFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}
