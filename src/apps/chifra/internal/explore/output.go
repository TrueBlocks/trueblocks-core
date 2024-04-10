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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunExplore handles the explore command for the command line. Returns error only as per cobra.
func RunExplore(cmd *cobra.Command, args []string) error {
	opts := exploreFinishParse(args)
	outputHelpers.EnableCommand("explore", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("explore", &opts.Globals)
	return opts.ExploreInternal()
}

// ServeExplore handles the explore command for the API. Returns an error.
func ServeExplore(w http.ResponseWriter, r *http.Request) error {
	opts := exploreFinishParseApi(w, r)
	outputHelpers.EnableCommand("explore", true)
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
	err = opts.HandleExplore()
	// EXISTING_CODE
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

	url := config.GetChain(chain).RemoteExplorer
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
		url = config.GetChain(chain).LocalExplorer
		query = strings.Replace(query, "tx/", "explorer/transactions/", -1)
		query = strings.Replace(query, "block/", "explorer/blocks/", -1)
	}

	return url + query
}

// EXISTING_CODE
