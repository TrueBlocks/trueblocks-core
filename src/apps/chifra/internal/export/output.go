// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
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
func RunExport(cmd *cobra.Command, args []string) (err error) {
	opts := exportFinishParse(args)
	outputHelpers.SetEnabledForCmds("export", opts.IsPorted())
	outputHelpers.SetWriterForCommand("export", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.ExportInternal()
	return
}

// ServeExport handles the export command for the API. Returns error and a bool if handled
func ServeExport(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := exportFinishParseApi(w, r)
	outputHelpers.SetEnabledForCmds("export", opts.IsPorted())
	outputHelpers.InitJsonWriterApi("export", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.ExportInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("export", err, &opts.Globals)
	return
}

// ExportInternal handles the internal workings of the export command.  Returns error and a bool if handled
func (opts *ExportOptions) ExportInternal() (err error, handled bool) {
	err = opts.validateExport()
	if err != nil {
		return err, true
	}

	timer := logger.NewTimer()
	msg := "chifra export"
	// EXISTING_CODE
	monitorArray := make([]monitor.Monitor, 0, len(opts.Addrs))
	if canceled, err := opts.FreshenMonitorsForExport(&monitorArray); err != nil || canceled {
		return err, true
	}

	if !opts.IsPorted() {
		logger.Fatal("Should not happen in BlocksInternal")
	}

	handled = true
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
	} else if opts.Appearances {
		err = opts.HandleAppearances(monitorArray)
	} else if opts.Statements {
		err = opts.HandleStatements(monitorArray)
	} else if opts.Balances {
		err = opts.HandleBalances(monitorArray)
	} else if opts.Neighbors {
		err = opts.HandleNeighbors(monitorArray)
	} else if opts.Accounting {
		err = opts.HandleAccounting(monitorArray)
	} else {
		err = opts.HandleShow(monitorArray)
	}
	// EXISTING_CODE
	timer.Report(msg)

	return
}

// GetExportOptions returns the options for this tool so other tools may use it.
func GetExportOptions(args []string, g *globals.GlobalOptions) *ExportOptions {
	ret := exportFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *ExportOptions) IsPorted() (ported bool) {
	// EXISTING_CODE
	ported = true
	// EXISTING_CODE
	return
}

// EXISTING_CODE
//

// Validate calls into the opts validateExport routine
func (opts *ExportOptions) Validate() error {
	return opts.validateExport()
}

// TODO: If an abi file is newer than the monitor file - clear the cache
// TODO: accounting disallows freshen, apps, logs, receipts, statements, traces, but requires articulate
// TODO: accounting must be for one monitor address - why?
// TODO: accounting requires node balances - why?
// TODO: Used to do this: if any ABI files was newer, re-read abi and re-articulate in cache
// TODO: Reconciliation loads traces -- plus it reduplicates the isSuicide, isGeneration, isUncle shit
// TODO: If a monitor file is locked, remove the lock and move on (don't read) but don't wait either

// TODO: In the old C++ code, we used to be able to customize the display of the output with a configuration string. This is a VERY important feature as it captures users
// TODO: In the old C++ code, the first address on the command line was `accountedFor`. Is that still true? Or do we now do accounting for multiple addresses? There should be testing.
// TODO: In the old C++ code, we used to load knownABIs if we were articulating. Is this still true? Do we load the known ABIs and then overlay them with contract specific clashes? (We should.)
// TODO: In the old C++ code, we used to be able to configure certain things - for example, `--cache` is on by default for all queries, `--cache_traces` is on by default, display strings, max_records, max_traces for dDos protection, etc.
// TODO: Need much better testing surrounding fourBytes, topics, emitters, relevant, etc. It's also very poorly documented.
// TODO: In the old C++ code, the ArticulateAll routine used to identify transactions as token related. Do we still do that? Must we? Why did we do that?

// EXISTING_CODE
