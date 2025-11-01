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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunExport handles the export command for the command line. Returns error only as per cobra.
func RunExport(cmd *cobra.Command, args []string) error {
	_ = cmd
	opts := exportFinishParse(args)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("export", &opts.Globals)
	return opts.ExportInternal(rCtx)
}

// ServeExport handles the export command for the API. Returns an error.
func ServeExport(w http.ResponseWriter, r *http.Request) error {
	opts := exportFinishParseApi(w, r)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("export", w, &opts.Globals)
	err := opts.ExportInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("export", err, &opts.Globals)
	return err
}

// ExportInternal handles the internal workings of the export command. Returns an error.
func (opts *ExportOptions) ExportInternal(rCtx *output.RenderCtx) error {
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
		err = opts.HandleDecache(rCtx, monitorArray)
	} else if opts.Count {
		err = opts.HandleCount(rCtx, monitorArray)
	} else if opts.Receipts {
		err = opts.HandleReceipts(rCtx, monitorArray)
	} else if opts.Logs {
		err = opts.HandleLogs(rCtx, monitorArray)
	} else if opts.Withdrawals {
		err = opts.HandleWithdrawals(rCtx, monitorArray)
	} else if opts.Appearances {
		err = opts.HandleAppearances(rCtx, monitorArray)
	} else if opts.Assets {
		err = opts.HandleAssets(rCtx, monitorArray)
	} else if opts.Balances {
		err = opts.HandleBalances(rCtx, monitorArray)
	} else if opts.Neighbors {
		err = opts.HandleNeighbors(rCtx, monitorArray)
	} else if opts.Statements {
		err = opts.HandleStatements(rCtx, monitorArray)
	} else if opts.Transfers {
		err = opts.HandleTransfers(rCtx, monitorArray)
	} else if opts.Traces {
		err = opts.HandleTraces(rCtx, monitorArray)
	} else if opts.Approvals {
		err = opts.HandleApprovals(rCtx, monitorArray)
	} else {
		err = opts.HandleShow(rCtx, monitorArray)
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
