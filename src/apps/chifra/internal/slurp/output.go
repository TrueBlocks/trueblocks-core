// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package slurpPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunSlurp handles the slurp command for the command line. Returns error only as per cobra.
func RunSlurp(cmd *cobra.Command, args []string) error {
	opts := slurpFinishParse(args)
	outputHelpers.EnableCommand("slurp", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("slurp", &opts.Globals)
	return opts.SlurpInternal()
}

// ServeSlurp handles the slurp command for the API. Returns an error.
func ServeSlurp(w http.ResponseWriter, r *http.Request) error {
	opts := slurpFinishParseApi(w, r)
	outputHelpers.EnableCommand("slurp", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("slurp", w, &opts.Globals)
	err := opts.SlurpInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("slurp", err, &opts.Globals)
	return err
}

// SlurpInternal handles the internal workings of the slurp command. Returns an error.
func (opts *SlurpOptions) SlurpInternal() error {
	var err error
	if err = opts.validateSlurp(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra slurp"
	// EXISTING_CODE
	if opts.Globals.Decache {
		err = opts.HandleDecache()
	} else if opts.Count {
		if opts.Source == "key" {
			err = opts.HandleCountKey()
		} else {
			err = opts.HandleCount()
		}
	} else if opts.Appearances {
		if opts.Source == "key" {
			err = opts.HandleAppearancesKey()
		} else {
			err = opts.HandleAppearances()
		}
	} else {
		err = opts.HandleShow()
	}
	// EXISTING_CODE
	timer.Report(msg)

	return err
}

// GetSlurpOptions returns the options for this tool so other tools may use it.
func GetSlurpOptions(args []string, g *globals.GlobalOptions) *SlurpOptions {
	ret := slurpFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

// EXISTING_CODE
// EXISTING_CODE
