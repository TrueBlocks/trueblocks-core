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
	"log"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
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
	log.Fatal("Should not happen. Daemon is an invalid route for server")
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
	handled = true
	apiUrl := opts.Port
	if !strings.HasPrefix(apiUrl, "http") {
		apiUrl = "http://localhost" + apiUrl
	}

	pad := func(strIn string) string {
		return utils.PadRight(strIn, 18, ' ')
	}

	chain := opts.Globals.Chain
	meta, err := rpcClient.GetMetaData(chain, false)
	logger.Log(logger.InfoC, pad("Server URL:"), apiUrl)
	logger.Log(logger.InfoC, pad("RPC Provider:"), config.GetRpcProvider(chain))
	logger.Log(logger.InfoC, pad("Root Config Path:"), config.GetPathToRootConfig())
	logger.Log(logger.InfoC, pad("Chain Config Path:"), config.GetPathToChainConfig(chain))
	logger.Log(logger.InfoC, pad("Cache Path:"), config.GetPathToCache(chain))
	logger.Log(logger.InfoC, pad("Index Path:"), config.GetPathToIndex(chain))
	if err != nil {
		msg := fmt.Sprintf("%sCould not load RPC provider: %s%s", colors.Red, err, colors.Off)
		logger.Log(logger.InfoC, pad("Progress:"), msg)
		log.Fatalf("")
	} else {
		nTs, _ := tslib.NTimestamps(opts.Globals.Chain)
		msg := fmt.Sprintf("%d, %d, %d,  %d, ts: %d", meta.Latest, meta.Finalized, meta.Staging, meta.Unripe, nTs)
		logger.Log(logger.InfoC, pad("Progress:"), msg)
	}

	// Start listening to the web sockets
	RunWebsocketPool()
	// Start listening for requests
	log.Fatal(http.ListenAndServe(opts.Port, NewRouter()))

	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE
