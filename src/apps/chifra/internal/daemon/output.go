// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package daemonPkg

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

// RunDaemon handles the daemon command for the command line. Returns error only as per cobra.
func RunDaemon(cmd *cobra.Command, args []string) (err error) {
	opts := daemonFinishParse(args)
	outputHelpers.SetWriterForCommand("daemon", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.DaemonInternal()
	return
}

// ServeDaemon handles the daemon command for the API. Returns error and a bool if handled
func ServeDaemon(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := daemonFinishParseApi(w, r)
	outputHelpers.InitJsonWriterApi("daemon", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.DaemonInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("daemon", err, &opts.Globals)
	return
}

// DaemonInternal handles the internal workings of the daemon command.  Returns error and a bool if handled
func (opts *DaemonOptions) DaemonInternal() (err error, handled bool) {
	err = opts.validateDaemon()
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

	return
}

// EXISTING_CODE
// EXISTING_CODE
