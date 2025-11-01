// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package configPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunConfig handles the config command for the command line. Returns error only as per cobra.
func RunConfig(cmd *cobra.Command, args []string) error {
	_ = cmd
	opts := configFinishParse(args)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("config", &opts.Globals)
	return opts.ConfigInternal(rCtx)
}

// ServeConfig handles the config command for the API. Returns an error.
func ServeConfig(w http.ResponseWriter, r *http.Request) error {
	opts := configFinishParseApi(w, r)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("config", w, &opts.Globals)
	err := opts.ConfigInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("config", err, &opts.Globals)
	return err
}

// ConfigInternal handles the internal workings of the config command. Returns an error.
func (opts *ConfigOptions) ConfigInternal(rCtx *output.RenderCtx) error {
	var err error
	if err = opts.validateConfig(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra config"
	// EXISTING_CODE
	// EXISTING_CODE
	if opts.Paths {
		err = opts.HandlePaths(rCtx)
	} else if opts.Dump {
		err = opts.HandleDump(rCtx)
	} else {
		err = opts.HandleShow(rCtx)
	}
	timer.Report(msg)

	return err
}

// GetConfigOptions returns the options for this tool so other tools may use it.
func GetConfigOptions(args []string, g *globals.GlobalOptions) *ConfigOptions {
	ret := configFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}
