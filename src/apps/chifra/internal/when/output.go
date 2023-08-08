// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package whenPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunWhen handles the when command for the command line. Returns error only as per cobra.
func RunWhen(cmd *cobra.Command, args []string) (err error) {
	opts := whenFinishParse(args)
	outputHelpers.SetEnabledForCmds("when", opts.IsPorted())
	outputHelpers.SetWriterForCommand("when", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.WhenInternal()
	return
}

// ServeWhen handles the when command for the API. Returns error and a bool if handled
func ServeWhen(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := whenFinishParseApi(w, r)
	outputHelpers.SetEnabledForCmds("when", opts.IsPorted())
	outputHelpers.InitJsonWriterApi("when", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.WhenInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("when", err, &opts.Globals)
	return
}

// WhenInternal handles the internal workings of the when command.  Returns error and a bool if handled
func (opts *WhenOptions) WhenInternal() (err error, handled bool) {
	err = opts.validateWhen()
	if err != nil {
		return err, true
	}

	timer := logger.NewTimer()
	msg := "chifra when"
	// EXISTING_CODE
	// TODO: This should use StreamMany for all cases
	handled = true

	if opts.Globals.Decache {
		err = opts.HandleDecache()

	} else if opts.List {
		err = opts.HandleList()

	} else if opts.Timestamps {
		if opts.Update {
			err = opts.HandleTimestampUpdate()

		} else if opts.Count {
			err = opts.HandleTimestampCount()

		} else if opts.Truncate != utils.NOPOS {
			err = opts.HandleTimestampsTruncate()

		} else {
			if opts.Check {
				err = opts.HandleTimestampsCheck()

			} else if opts.Repair {
				err = opts.HandleTimestampsRepair()

			} else {
				err = opts.HandleTimestampsShow()

			}
		}

	} else {
		err = opts.HandleShow()
	}
	// EXISTING_CODE
	timer.Report(msg)

	return
}

// GetWhenOptions returns the options for this tool so other tools may use it.
func GetWhenOptions(args []string, g *globals.GlobalOptions) *WhenOptions {
	ret := whenFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *WhenOptions) IsPorted() (ported bool) {
	// EXISTING_CODE
	ported = true
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
