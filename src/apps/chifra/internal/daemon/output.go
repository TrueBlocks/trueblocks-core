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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/spf13/cobra"
)

//lint:file-ignore SA4017 timer value below is auto generated, but not used
//lint:file-ignore SA4006 timer was auto generated but not used
// EXISTING_CODE

// RunDaemon handles the daemon command for the command line. Returns error only as per cobra.
func RunDaemon(cmd *cobra.Command, args []string) (err error) {
	opts := daemonFinishParse(args)
	outputHelpers.SetEnabledForCmds("daemon", opts.IsPorted())
	outputHelpers.SetWriterForCommand("daemon", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.DaemonInternal()
	return
}

// ServeDaemon handles the daemon command for the API. Returns error and a bool if handled
func ServeDaemon(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := daemonFinishParseApi(w, r)
	outputHelpers.SetEnabledForCmds("daemon", opts.IsPorted())
	outputHelpers.InitJsonWriterApi("daemon", w, &opts.Globals)
	// EXISTING_CODE
	logger.Fatal("Should not happen. Daemon is an invalid route for server")
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

	timer := logger.NewTimer()
	msg := "chifra daemon"
	// EXISTING_CODE
	if !opts.IsPorted() {
		logger.Fatal("Should not happen in DaemonInternal")
	}

	handled = true
	apiUrl := opts.Port
	if !strings.HasPrefix(apiUrl, "http") {
		apiUrl = "http://localhost" + apiUrl
	}

	chain := opts.Globals.Chain
	logger.InfoTable("Server URL:        ", apiUrl)
	logger.InfoTable("RPC Provider:      ", config.GetRpcProvider(chain))
	logger.InfoTable("Root Config Path:  ", config.GetPathToRootConfig())
	logger.InfoTable("Chain Config Path: ", config.GetPathToChainConfig(chain))
	logger.InfoTable("Cache Path:        ", config.GetPathToCache(chain))
	logger.InfoTable("Index Path:        ", config.GetPathToIndex(chain))

	meta, err := rpcClient.GetMetaData(chain, false)
	if err != nil {
		msg := fmt.Sprintf("%sCould not load RPC provider: %s%s", colors.Red, err, colors.Off)
		logger.InfoTable("Progress:", msg)
		logger.Fatal("")
	} else {
		nTs, _ := tslib.NTimestamps(opts.Globals.Chain)
		msg := fmt.Sprintf("%d, %d, %d, %d, ts: %d", meta.Latest, meta.Finalized, meta.Staging, meta.Unripe, nTs)
		logger.InfoTable("Progress:          ", msg)
	}

	go opts.HandleScraper()
	go opts.HandleMonitor()
	go opts.HandleGrpc()

	// Start listening to the web sockets
	RunWebsocketPool()
	// Start listening for requests
	logger.Fatal(http.ListenAndServe(opts.Port, NewRouter()))

	// EXISTING_CODE
	timer.Report(msg)

	return
}

// GetDaemonOptions returns the options for this tool so other tools may use it.
func GetDaemonOptions(args []string, g *globals.GlobalOptions) *DaemonOptions {
	ret := daemonFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *DaemonOptions) IsPorted() (ported bool) {
	// EXISTING_CODE
	ported = true
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
