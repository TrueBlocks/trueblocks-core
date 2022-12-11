// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package nodePkg

// EXISTING_CODE
import (
	"fmt"
	"net/http"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunNode handles the node command for the command line. Returns error only as per cobra.
func RunNode(cmd *cobra.Command, args []string) (err error) {
	opts := nodeFinishParse(args)
	outputHelpers.SetWriterForCommand("node", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.NodeInternal()
	return
}

// ServeNode handles the node command for the API. Returns error and a bool if handled
func ServeNode(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := nodeFinishParseApi(w, r)
	outputHelpers.InitJsonWriterApi("node", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.NodeInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("node", err, &opts.Globals)
	return
}

// NodeInternal handles the internal workings of the node command.  Returns error and a bool if handled
func (opts *NodeOptions) NodeInternal() (err error, handled bool) {
	err = opts.validateNode()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	cnt := 0
	for {
		fmt.Println("node scrape:", colors.BrightGreen, opts.Scrape, colors.Off, " monitors:", colors.BrightGreen, opts.Monitor, colors.Off, " api:", colors.BrightGreen, opts.Api, colors.Off, cnt)
		time.Sleep(time.Second * 4)
		cnt++
	}
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
