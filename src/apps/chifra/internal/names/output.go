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
			err = opts.HandlePrefundOnly()
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
	if opts.Clean || len(opts.Autoname) > 0 || len(opts.Globals.OutputFn) > 0 ||
		opts.Create || opts.Update || opts.Delete || opts.Undelete || opts.Remove {
		ported = false
	} else if len(opts.Terms) > 0 {
		ported = opts.Tags || opts.Addr
	} else {
		if opts.Tags || opts.Addr {
			ported = true
		} else if opts.Expand {
			ported = false
		} else if opts.All {
			ported = false
		} else if opts.Custom {
			ported = false
		} else if opts.Named {
			ported = false
		} else if opts.ToCustom {
			ported = false
		} else {
			ported = opts.Prefund
		}
	}
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
