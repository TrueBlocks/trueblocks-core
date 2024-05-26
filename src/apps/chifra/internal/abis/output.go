// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package abisPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunAbis handles the abis command for the command line. Returns error only as per cobra.
func RunAbis(cmd *cobra.Command, args []string) error {
	opts := abisFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("abis", &opts.Globals)
	return opts.AbisInternal()
}

// ServeAbis handles the abis command for the API. Returns an error.
func ServeAbis(w http.ResponseWriter, r *http.Request) error {
	opts := abisFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("abis", w, &opts.Globals)
	err := opts.AbisInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("abis", err, &opts.Globals)
	return err
}

// AbisInternal handles the internal workings of the abis command. Returns an error.
func (opts *AbisOptions) AbisInternal() error {
	var err error
	if err = opts.validateAbis(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra abis"
	// EXISTING_CODE
	// EXISTING_CODE
	if opts.Globals.Decache {
		err = opts.HandleDecache()
	} else if len(opts.Find) > 0 {
		err = opts.HandleFind()
	} else if len(opts.Encode) > 0 {
		err = opts.HandleEncode()
	} else {
		err = opts.HandleShow()
	}
	timer.Report(msg)

	return err
}

// GetAbisOptions returns the options for this tool so other tools may use it.
func GetAbisOptions(args []string, g *globals.GlobalOptions) *AbisOptions {
	ret := abisFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}
