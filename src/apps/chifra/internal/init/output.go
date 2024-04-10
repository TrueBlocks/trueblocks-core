// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package initPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunInit handles the init command for the command line. Returns error only as per cobra.
func RunInit(cmd *cobra.Command, args []string) error {
	opts := initFinishParse(args)
	outputHelpers.EnableCommand("init", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("init", &opts.Globals)
	return opts.InitInternal()
}

// ServeInit handles the init command for the API. Returns an error.
func ServeInit(w http.ResponseWriter, r *http.Request) error {
	opts := initFinishParseApi(w, r)
	outputHelpers.EnableCommand("init", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("init", w, &opts.Globals)
	err := opts.InitInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("init", err, &opts.Globals)
	return err
}

// InitInternal handles the internal workings of the init command.  Returns an error.
func (opts *InitOptions) InitInternal() error {
	var err error
	if err = opts.validateInit(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra init"
	// EXISTING_CODE
	if opts.DryRun {
		err = opts.HandleDryRun()
	} else {
		err = opts.HandleInit()
	}

	// EXISTING_CODE
	timer.Report(msg)

	return err
}

// GetInitOptions returns the options for this tool so other tools may use it.
func GetInitOptions(args []string, g *globals.GlobalOptions) *InitOptions {
	ret := initFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

// EXISTING_CODE
// EXISTING_CODE
