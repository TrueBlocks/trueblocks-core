// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package monitorsPkg

// EXISTING_CODE
import (
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunMonitors handles the monitors command for the command line. Returns error only as per cobra.
func RunMonitors(cmd *cobra.Command, args []string) (err error) {
	opts := monitorsFinishParse(args)
	outputHelpers.SetEnabledForCmds("monitors", opts.IsPorted())
	outputHelpers.SetWriterForCommand("monitors", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.MonitorsInternal()
	return
}

// ServeMonitors handles the monitors command for the API. Returns error and a bool if handled
func ServeMonitors(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := monitorsFinishParseApi(w, r)
	outputHelpers.SetEnabledForCmds("monitors", opts.IsPorted())
	outputHelpers.InitJsonWriterApi("monitors", w, &opts.Globals)
	// EXISTING_CODE
	if !opts.Globals.TestMode { // our test harness does not use DELETE
		delOptions := "--delete, --undelete, or --remove"
		if r.Method == "DELETE" {
			if !opts.Delete && !opts.Undelete && !opts.Remove {
				err = validate.Usage("Specify one of {0} when using the DELETE route.", delOptions)
			}
		} else {
			if opts.Delete || opts.Undelete || opts.Remove {
				delOptions = strings.Replace(delOptions, " or ", " and ", -1)
				err = validate.Usage("The {0} options are not valid when using the GET route.", delOptions)
			}
		}
		if err != nil {
			return err, true
		}
	}
	// EXISTING_CODE
	err, handled = opts.MonitorsInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("monitors", err, &opts.Globals)
	return
}

// MonitorsInternal handles the internal workings of the monitors command.  Returns error and a bool if handled
func (opts *MonitorsOptions) MonitorsInternal() (err error, handled bool) {
	err = opts.validateMonitors()
	if err != nil {
		return err, true
	}

	timer := logger.NewTimer()
	msg := "chifra monitors"
	// EXISTING_CODE
	handled = true // everything is handled even on failure

	if opts.Clean {
		err = opts.HandleClean()

	} else if opts.List {
		err = opts.HandleList()

	} else if opts.Watch {
		err = opts.HandleWatch()

	} else {
		err = opts.HandleCrudCommands()
	}
	// EXISTING_CODE
	timer.Report(msg)

	return
}

// GetMonitorsOptions returns the options for this tool so other tools may use it.
func GetMonitorsOptions(args []string, g *globals.GlobalOptions) *MonitorsOptions {
	ret := monitorsFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *MonitorsOptions) IsPorted() (ported bool) {
	// EXISTING_CODE
	ported = opts.List // Don't change this line. (Try it. See what happens.)
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
