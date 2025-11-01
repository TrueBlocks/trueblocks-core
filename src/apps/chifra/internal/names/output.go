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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunNames handles the names command for the command line. Returns error only as per cobra.
func RunNames(cmd *cobra.Command, args []string) error {
	_ = cmd
	opts := namesFinishParse(args)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	var err1 error
	if err1 = opts.LoadCrudDataIfNeeded(nil); err1 != nil {
		return err1
	}
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("names", &opts.Globals)
	return opts.NamesInternal(rCtx)
}

// ServeNames handles the names command for the API. Returns an error.
func ServeNames(w http.ResponseWriter, r *http.Request) error {
	opts := namesFinishParseApi(w, r)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	var err1 error
	if err1 = opts.LoadCrudDataIfNeeded(r); err1 != nil {
		return err1
	}
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("names", w, &opts.Globals)
	err := opts.NamesInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("names", err, &opts.Globals)
	return err
}

// NamesInternal handles the internal workings of the names command. Returns an error.
func (opts *NamesOptions) NamesInternal(rCtx *output.RenderCtx) error {
	var err error
	if err = opts.validateNames(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra names"
	// EXISTING_CODE
	// EXISTING_CODE
	if opts.Count {
		err = opts.HandleCount(rCtx)
	} else if len(opts.Autoname) > 0 {
		err = opts.HandleAutoname(rCtx)
	} else if opts.Clean {
		err = opts.HandleClean(rCtx)
	} else if opts.Tags {
		err = opts.HandleTags(rCtx)
	} else if opts.anyCrud() {
		err = opts.HandleCrud(rCtx)
	} else {
		err = opts.HandleShow(rCtx)
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
