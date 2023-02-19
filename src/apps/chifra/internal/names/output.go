// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package namesPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunNames handles the names command for the command line. Returns error only as per cobra.
func RunNames(cmd *cobra.Command, args []string) (err error) {
	opts := namesFinishParse(args)
	outputHelpers.SetEnabledForCmds("names", opts.IsPorted())
	outputHelpers.SetWriterForCommand("names", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.NamesInternal()
	return
}

// ServeNames handles the names command for the API. Returns error and a bool if handled
func ServeNames(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := namesFinishParseApi(w, r)
	outputHelpers.SetEnabledForCmds("names", opts.IsPorted())
	outputHelpers.InitJsonWriterApi("names", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.NamesInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("names", err, &opts.Globals)
	return
}

// NamesInternal handles the internal workings of the names command.  Returns error and a bool if handled
func (opts *NamesOptions) NamesInternal() (err error, handled bool) {
	err = opts.validateNames()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	if opts.IsPorted() {
		handled = true
		if opts.Tags {
			err = opts.HandleTags()
		} else if opts.Addr {
			err = opts.HandleAddr()
		} else {
			// Includes opts.Prefund
			err = opts.HandleTerms()
		}
		return
	}

	if opts.Globals.IsApiMode() {
		return nil, false
	}

	handled = true
	err = opts.Globals.PassItOn("ethNames", opts.Globals.Chain, opts.toCmdLine(), opts.getEnvStr())
	// EXISTING_CODE

	return
}

// GetNamesOptions returns the options for this tool so other tools may use it.
func GetNamesOptions(args []string, g *globals.GlobalOptions) *NamesOptions {
	ret := namesFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *NamesOptions) IsPorted() (ported bool) {
	// EXISTING_CODE
	if opts.Clean || len(opts.Autoname) > 0 || opts.ToCustom ||
		opts.Create || opts.Update || opts.Delete || opts.Undelete || opts.Remove {
		ported = false
	} else {
		ported = true
	}
	// EXISTING_CODE
	return
}

// EXISTING_CODE
func (opts *NamesOptions) getType() names.Parts {
	var ret names.Parts

	if opts.Custom || opts.All {
		ret |= names.Custom
	}

	if opts.Prefund || opts.All {
		ret |= names.Prefund
	}

	if (!opts.Custom && !opts.Prefund) || opts.All {
		ret |= names.Regular
	}

	if opts.MatchCase {
		ret |= names.MatchCase
	}

	if opts.Expand {
		ret |= names.Expanded
	}

	if opts.Globals.TestMode {
		ret |= names.Testing
	}

	return ret
}

// EXISTING_CODE
