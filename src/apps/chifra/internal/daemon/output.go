// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package daemonPkg

// EXISTING_CODE
import (
	"fmt"
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunDaemon handles the daemon command for the command line. Returns error only as per cobra.
func RunDaemon(cmd *cobra.Command, args []string) error {
	opts := daemonFinishParse(args)
	outputHelpers.EnableCommand("daemon", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("daemon", &opts.Globals)
	return opts.DaemonInternal()
}

// ServeDaemon handles the daemon command for the API. Returns an error.
func ServeDaemon(w http.ResponseWriter, r *http.Request) error {
	opts := daemonFinishParseApi(w, r)
	outputHelpers.EnableCommand("daemon", true)
	// EXISTING_CODE
	if true { // defeats linter
		logger.Fatal("should not happen ==> Daemon is an invalid route for server")
	}
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("daemon", w, &opts.Globals)
	err := opts.DaemonInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("daemon", err, &opts.Globals)
	return err
}

// DaemonInternal handles the internal workings of the daemon command. Returns an error.
func (opts *DaemonOptions) DaemonInternal() error {
	var err error
	if err = opts.validateDaemon(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra daemon"
	// EXISTING_CODE
	chain := opts.Globals.Chain
	provider := config.GetChain(chain).RpcProvider

	logger.InfoTable("Server URL:        ", opts.Url)
	logger.InfoTable("RPC Provider:      ", provider)
	logger.InfoTable("Root Config Path:  ", config.PathToRootConfig())
	logger.InfoTable("Chain Config Path: ", config.MustGetPathToChainConfig(chain))
	logger.InfoTable("Cache Path:        ", config.PathToCache(chain))
	logger.InfoTable("Index Path:        ", config.PathToIndex(chain))

	meta, err := opts.Conn.GetMetaData(false)
	if err != nil {
		msg := fmt.Sprintf("%sCould not load RPC provider: %s%s", colors.Red, err, colors.Off)
		logger.InfoTable("Progress:", msg)
		logger.Fatal("")
	} else {
		nTs, _ := tslib.NTimestamps(chain) // when the file has one record, the block is zero, etc.
		if nTs > 0 {
			nTs--
		}
		msg := fmt.Sprintf("%d, %d, %d, %d, ts: %d", meta.Latest, meta.Finalized, meta.Staging, meta.Unripe, nTs)
		logger.InfoTable("Progress:          ", msg)
	}

	go func() {
		_ = opts.HandleScraper()
	}()
	go func() {
		_ = opts.HandleMonitor()
	}()
	go func() {
		_ = opts.HandleGrpc()
	}()

	// do not remove, this fixes a lint warning that happens in the boilerplate because of the Fatal just below
	timer.Report(msg)

	// Start listening to the web sockets
	RunWebsocketPool()
	// Start listening for requests
	logger.Fatal(http.ListenAndServe(opts.Url, NewRouter()))

	// EXISTING_CODE
	timer.Report(msg)

	return err
}

// GetDaemonOptions returns the options for this tool so other tools may use it.
func GetDaemonOptions(args []string, g *globals.GlobalOptions) *DaemonOptions {
	ret := daemonFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

// EXISTING_CODE
// EXISTING_CODE
