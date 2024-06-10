// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package namesPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunNames handles the names command for the command line. Returns error only as per cobra.
func RunNames(cmd *cobra.Command, args []string) error {
	opts := namesFinishParse(args)
	// EXISTING_CODE
	var err1 error
	if err1 = opts.LoadCrudDataIfNeeded(nil); err1 != nil {
		return err1
	}
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("names", &opts.Globals)
	return opts.NamesInternal()
}

// ServeNames handles the names command for the API. Returns an error.
func ServeNames(w http.ResponseWriter, r *http.Request) error {
	opts := namesFinishParseApi(w, r)
	// EXISTING_CODE
	var err1 error
	if err1 = opts.LoadCrudDataIfNeeded(r); err1 != nil {
		return err1
	}
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("names", w, &opts.Globals)
	err := opts.NamesInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("names", err, &opts.Globals)
	return err
}

// NamesInternal handles the internal workings of the names command. Returns an error.
func (opts *NamesOptions) NamesInternal() error {
	var err error
	if err = opts.validateNames(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra names"
	// EXISTING_CODE
	// EXISTING_CODE
	if len(opts.Autoname) > 0 {
		err = opts.HandleAutoname()
	} else if opts.Clean {
		err = opts.HandleClean()
	} else if opts.Tags {
		err = opts.HandleTags()
	} else if opts.anyCrud() {
		err = opts.HandleCrud()
	} else {
		err = opts.HandleShow()
	}
	timer.Report(msg)

	return err
}

// GetNamesOptions returns the options for this tool so other tools may use it.
func GetNamesOptions(args []string, g *globals.GlobalOptions) *NamesOptions {
	ret := namesFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *NamesOptions) anyCrud() bool {
	return opts.Create ||
		opts.Update ||
		opts.Delete ||
		opts.Undelete ||
		opts.Remove
}
