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

	// EXISTING_CODE
	_, err = opts.FreshenMonitorsForExport()
	if err != nil {
		return err, true
	}

	if opts.Globals.IsApiMode() {
		// The caller has to handle this when in API mode
		return nil, false
	}

	handled = true
	err = opts.Globals.PassItOn("acctExport", opts.Globals.Chain, opts.toCmdLine(), opts.getEnvStr())
	// EXISTING_CODE

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
	ported = false
	// EXISTING_CODE
	return
}

// EXISTING_CODE
func (opts *ExportOptions) Validate() error {
	return opts.validateExport()
}

// EXISTING_CODE
