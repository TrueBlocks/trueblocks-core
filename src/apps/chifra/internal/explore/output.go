// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package explorePkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunExplore handles the explore command for the command line. Returns error only as per cobra.
func RunExplore(cmd *cobra.Command, args []string) error {
	opts := exploreFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("explore", &opts.Globals)
	return opts.ExploreInternal()
}

// ServeExplore handles the explore command for the API. Returns an error.
func ServeExplore(w http.ResponseWriter, r *http.Request) error {
	opts := exploreFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("explore", w, &opts.Globals)
	err := opts.ExploreInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("explore", err, &opts.Globals)
	return err
}

// ExploreInternal handles the internal workings of the explore command. Returns an error.
func (opts *ExploreOptions) ExploreInternal() error {
	var err error
	if err = opts.validateExplore(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra explore"
	// EXISTING_CODE
	// EXISTING_CODE
	err = opts.HandleShow()
	timer.Report(msg)

	return err
}

// GetExploreOptions returns the options for this tool so other tools may use it.
func GetExploreOptions(args []string, g *globals.GlobalOptions) *ExploreOptions {
	ret := exploreFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}
