// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package logsPkg

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

// RunLogs handles the logs command for the command line. Returns error only as per cobra.
func RunLogs(cmd *cobra.Command, args []string) error {
	opts := logsFinishParse(args)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("logs", &opts.Globals)
	return opts.LogsInternal(rCtx)
}

// ServeLogs handles the logs command for the API. Returns an error.
func ServeLogs(w http.ResponseWriter, r *http.Request) error {
	opts := logsFinishParseApi(w, r)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("logs", w, &opts.Globals)
	err := opts.LogsInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("logs", err, &opts.Globals)
	return err
}

// LogsInternal handles the internal workings of the logs command. Returns an error.
func (opts *LogsOptions) LogsInternal(rCtx output.RenderCtx) error {
	var err error
	if err = opts.validateLogs(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra logs"
	// EXISTING_CODE
	// EXISTING_CODE
	if opts.Globals.Decache {
		err = opts.HandleDecache(rCtx)
	} else {
		err = opts.HandleShow(rCtx)
	}
	timer.Report(msg)

	return err
}

// GetLogsOptions returns the options for this tool so other tools may use it.
func GetLogsOptions(args []string, g *globals.GlobalOptions) *LogsOptions {
	ret := logsFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}
