// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package logsPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunLogs handles the logs command for the command line. Returns error only as per cobra.
func RunLogs(cmd *cobra.Command, args []string) (err error) {
	opts := logsFinishParse(args)
	outputHelpers.SetEnabledForCmds("logs", opts.IsPorted())
	outputHelpers.SetWriterForCommand("logs", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.LogsInternal()
	return
}

// ServeLogs handles the logs command for the API. Returns error and a bool if handled
func ServeLogs(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := logsFinishParseApi(w, r)
	outputHelpers.SetEnabledForCmds("logs", opts.IsPorted())
	outputHelpers.InitJsonWriterApi("logs", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.LogsInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("logs", err, &opts.Globals)
	return
}

// LogsInternal handles the internal workings of the logs command.  Returns error and a bool if handled
func (opts *LogsOptions) LogsInternal() (err error, handled bool) {
	err = opts.validateLogs()
	if err != nil {
		return err, true
	}

	timer := logger.NewTimer()
	msg := "chifra logs"
	// EXISTING_CODE
	if !opts.IsPorted() {
		logger.Panic("Should not happen in chifra logs.")
	}

	handled = true
	if opts.Globals.Decache {
		err = opts.HandleDecache()
	} else {
		err = opts.HandleShow()
	}
	// EXISTING_CODE
	timer.Report(msg)

	return
}

// GetLogsOptions returns the options for this tool so other tools may use it.
func GetLogsOptions(args []string, g *globals.GlobalOptions) *LogsOptions {
	ret := logsFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *LogsOptions) IsPorted() (ported bool) {
	// EXISTING_CODE
	ported = true
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
