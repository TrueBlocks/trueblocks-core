// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package configPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunConfig handles the config command for the command line. Returns error only as per cobra.
func RunConfig(cmd *cobra.Command, args []string) (err error) {
	opts := configFinishParse(args)
	outputHelpers.SetEnabledForCmds("config", opts.IsPorted())
	outputHelpers.SetWriterForCommand("config", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.ConfigInternal()
	return
}

// ServeConfig handles the config command for the API. Returns error and a bool if handled
func ServeConfig(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := configFinishParseApi(w, r)
	outputHelpers.SetEnabledForCmds("config", opts.IsPorted())
	outputHelpers.InitJsonWriterApi("config", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.ConfigInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("config", err, &opts.Globals)
	return
}

// ConfigInternal handles the internal workings of the config command.  Returns error and a bool if handled
func (opts *ConfigOptions) ConfigInternal() (err error, handled bool) {
	err = opts.validateConfig()
	if err != nil {
		return err, true
	}

	timer := logger.NewTimer()
	msg := "chifra config"
	// EXISTING_CODE
	if !opts.IsPorted() {
		logger.Fatal("Should not happen.")
	}

	handled = true
	if opts.Paths {
		err = opts.HandlePaths()
	} else {
		logger.Warn("The config tool is current unavailable. Please use the 'chifra status' tool instead.")
	}
	// EXISTING_CODE
	timer.Report(msg)

	return
}

// GetConfigOptions returns the options for this tool so other tools may use it.
func GetConfigOptions(args []string, g *globals.GlobalOptions) *ConfigOptions {
	ret := configFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *ConfigOptions) IsPorted() (ported bool) {
	// EXISTING_CODE
	ported = true
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
