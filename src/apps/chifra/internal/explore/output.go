// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package explorePkg

// EXISTING_CODE
import (
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunExplore handles the explore command for the command line. Returns error only as per cobra.
func RunExplore(cmd *cobra.Command, args []string) (err error) {
	opts := exploreFinishParse(args)
	outputHelpers.SetEnabledForCmds("explore", opts.IsPorted())
	outputHelpers.SetWriterForCommand("explore", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.ExploreInternal()
	return
}

// ServeExplore handles the explore command for the API. Returns error and a bool if handled
func ServeExplore(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := exploreFinishParseApi(w, r)
	outputHelpers.SetEnabledForCmds("explore", opts.IsPorted())
	outputHelpers.InitJsonWriterApi("explore", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.ExploreInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("explore", err, &opts.Globals)
	return
}

// ExploreInternal handles the internal workings of the explore command.  Returns error and a bool if handled
func (opts *ExploreOptions) ExploreInternal() (err error, handled bool) {
	err = opts.validateExplore()
	if err != nil {
		return err, true
	}

	timer := logger.NewTimer()
	msg := "chifra explore"
	// EXISTING_CODE
	if !opts.IsPorted() {
		logger.Fatal("Should not happen in ExploreInternal")
	}

	handled = true
	err = opts.HandleExplore()
	// EXISTING_CODE
	timer.Report(msg)

	return
}

// GetExploreOptions returns the options for this tool so other tools may use it.
func GetExploreOptions(args []string, g *globals.GlobalOptions) *ExploreOptions {
	ret := exploreFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *ExploreOptions) IsPorted() (ported bool) {
	// EXISTING_CODE
	ported = true
	// EXISTING_CODE
	return
}

// EXISTING_CODE
func (u *ExploreUrl) getUrl(opts *ExploreOptions) string {

	var chain = opts.Globals.Chain

	if opts.Google {
		var query = "https://www.google.com/search?q=[{TERM}]"
		query = strings.Replace(query, "[{TERM}]", u.term, -1)
		var exclusions = []string{
			"etherscan", "etherchain", "bloxy", "bitquery", "ethplorer", "tokenview", "anyblocks", "explorer",
		}
		for _, ex := range exclusions {
			query += ("+-" + ex)
		}
		return query
	}

	if u.termType == ExploreFourByte {
		var query = "https://www.4byte.directory/signatures/?bytes4_signature=[{TERM}]"
		query = strings.Replace(query, "[{TERM}]", u.term, -1)
		return query
	}

	if u.termType == ExploreEnsName {
		var query = "https://app.ens.domains/name/[{TERM}]/details"
		query = strings.Replace(query, "[{TERM}]", u.term, -1)
		return query
	}

	url := config.GetRemoteExplorer(chain)
	query := ""
	switch u.termType {
	case ExploreNone:
		// do nothing
	case ExploreTx:
		query = "tx/" + u.term
	case ExploreBlock:
		query = "block/" + u.term
	case ExploreAddress:
		fallthrough
	default:
		query = "address/" + u.term
	}

	if opts.Local {
		url = config.GetLocalExplorer(chain)
		query = strings.Replace(query, "tx/", "explorer/transactions/", -1)
		query = strings.Replace(query, "block/", "explorer/blocks/", -1)
	}

	return url + query
}

// EXISTING_CODE
