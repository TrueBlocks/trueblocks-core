// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package statusPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunStatus handles the status command for the command line. Returns error only as per cobra.
func RunStatus(cmd *cobra.Command, args []string) error {
	opts := statusFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("status", &opts.Globals)
	return opts.StatusInternal()
}

// ServeStatus handles the status command for the API. Returns an error.
func ServeStatus(w http.ResponseWriter, r *http.Request) error {
	opts := statusFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("status", w, &opts.Globals)
	err := opts.StatusInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("status", err, &opts.Globals)
	return err
}

// StatusInternal handles the internal workings of the status command. Returns an error.
func (opts *StatusOptions) StatusInternal() error {
	var err error
	if err = opts.validateStatus(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra status"
	// EXISTING_CODE
	if len(opts.ModeTypes) > 0 {
		err = opts.HandleCaches()
	} else if opts.Diagnose {
		err = opts.HandleDiagnose()
	} else {
		err = opts.HandleShow()
	}
	// EXISTING_CODE
	timer.Report(msg)

	return err
}

// GetStatusOptions returns the options for this tool so other tools may use it.
func GetStatusOptions(args []string, g *globals.GlobalOptions) *StatusOptions {
	ret := statusFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

// EXISTING_CODE
// EXISTING_CODE
