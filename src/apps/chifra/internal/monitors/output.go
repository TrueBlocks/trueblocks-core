// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package monitorsPkg

// EXISTING_CODE
import (
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunMonitors handles the monitors command for the command line. Returns error only as per cobra.
func RunMonitors(cmd *cobra.Command, args []string) error {
	_ = cmd
	opts := monitorsFinishParse(args)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("monitors", &opts.Globals)
	return opts.MonitorsInternal(rCtx)
}

// ServeMonitors handles the monitors command for the API. Returns an error.
func ServeMonitors(w http.ResponseWriter, r *http.Request) error {
	opts := monitorsFinishParseApi(w, r)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// TODO: can we move this to Validate?
	var err1 error
	if !opts.Globals.TestMode { // our test harness does not use DELETE
		delOptions := "--delete, --undelete, or --remove"
		if r.Method == "DELETE" {
			if !opts.Delete && !opts.Undelete && !opts.Remove {
				err1 = validate.Usage("Specify one of {0} when using the DELETE route.", delOptions)
			}
		} else {
			if opts.Delete || opts.Undelete || opts.Remove {
				delOptions = strings.Replace(delOptions, " or ", " and ", -1)
				err1 = validate.Usage("The {0} options are not valid when using the GET route.", delOptions)
			}
		}
		if err1 != nil {
			return err1
		}
	}
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("monitors", w, &opts.Globals)
	err := opts.MonitorsInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("monitors", err, &opts.Globals)
	return err
}

// MonitorsInternal handles the internal workings of the monitors command. Returns an error.
func (opts *MonitorsOptions) MonitorsInternal(rCtx *output.RenderCtx) error {
	var err error
	if err = opts.validateMonitors(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra monitors"
	// EXISTING_CODE
	// EXISTING_CODE
	if opts.Globals.Decache {
		err = opts.HandleDecache(rCtx)
	} else if opts.Count {
		err = opts.HandleCount(rCtx)
	} else if opts.Clean {
		err = opts.HandleClean(rCtx)
	} else if opts.List {
		err = opts.HandleList(rCtx)
	} else if opts.anyCrud() {
		err = opts.HandleCrud(rCtx)
	} else {
		err = opts.HandleShow(rCtx)
	}
	timer.Report(msg)

	return err
}

// GetMonitorsOptions returns the options for this tool so other tools may use it.
func GetMonitorsOptions(args []string, g *globals.GlobalOptions) *MonitorsOptions {
	ret := monitorsFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *MonitorsOptions) anyCrud() bool {
	return opts.Delete ||
		opts.Undelete ||
		opts.Remove
}
