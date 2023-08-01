// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package daemonPkg

import (
	"encoding/json"
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// DaemonOptions provides all command options for the chifra daemon command.
type DaemonOptions struct {
	Port    string                `json:"port,omitempty"`    // Specify the server's port
	Api     string                `json:"api,omitempty"`     // Instruct the node to start the API server
	Scrape  string                `json:"scrape,omitempty"`  // Start the scraper, initialize it with either just blooms or entire index, generate for new blocks
	Monitor bool                  `json:"monitor,omitempty"` // Instruct the node to start the monitors tool
	Grpc    bool                  `json:"grpc,omitempty"`    // Run gRPC server to serve names
	Globals globals.GlobalOptions `json:"globals,omitempty"` // The global options
	Conn    *rpcClient.Options    `json:"conn,omitempty"`    // The connection to the RPC server
	BadFlag error                 `json:"badFlag,omitempty"` // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultDaemonOptions = DaemonOptions{
	Port: ":8080",
	Api:  "on",
}

// testLog is used only during testing to export the options for this test case.
func (opts *DaemonOptions) testLog() {
	logger.TestLog(len(opts.Port) > 0, "Port: ", opts.Port)
	logger.TestLog(len(opts.Api) > 0, "Api: ", opts.Api)
	logger.TestLog(len(opts.Scrape) > 0, "Scrape: ", opts.Scrape)
	logger.TestLog(opts.Monitor, "Monitor: ", opts.Monitor)
	logger.TestLog(opts.Grpc, "Grpc: ", opts.Grpc)
	opts.Conn.TestLog()
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *DaemonOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// daemonFinishParseApi finishes the parsing for server invocations. Returns a new DaemonOptions.
func daemonFinishParseApi(w http.ResponseWriter, r *http.Request) *DaemonOptions {
	copy := defaultDaemonOptions
	opts := &copy
	for key, value := range r.URL.Query() {
		switch key {
		case "port":
			opts.Port = value[0]
		case "api":
			opts.Api = value[0]
		case "scrape":
			opts.Scrape = value[0]
		case "monitor":
			opts.Monitor = true
		case "grpc":
			opts.Grpc = true
		default:
			if !copy.Globals.Caps.HasKey(key) {
				opts.Conn = &rpcClient.Options{}
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "daemon")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	chain := opts.Globals.Chain
	caches := []string{}
	opts.Conn = rpcClient.NewConnection(chain, caches)

	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

// daemonFinishParse finishes the parsing for command line invocations. Returns a new DaemonOptions.
func daemonFinishParse(args []string) *DaemonOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	chain := opts.Globals.Chain
	caches := []string{}
	opts.Conn = rpcClient.NewConnection(chain, caches)

	// EXISTING_CODE
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}

	return opts
}

func GetOptions() *DaemonOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultDaemonOptions
}

func ResetOptions() {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultDaemonOptions = DaemonOptions{}
	globals.SetDefaults(&defaultDaemonOptions.Globals)
	defaultDaemonOptions.Globals.Writer = w
	capabilities := caps.Default // Additional global caps for chifra daemon
	// EXISTING_CODE
	capabilities = capabilities.Remove(caps.Chain)
	capabilities = capabilities.Remove(caps.NoHeader)
	capabilities = capabilities.Remove(caps.Output)
	capabilities = capabilities.Remove(caps.Append)
	// capabilities = capabilities.Remove(caps.Fmt)
	// capabilities = capabilities.Remove(caps.File)
	// EXISTING_CODE
	defaultDaemonOptions.Globals.Caps = capabilities
}

// EXISTING_CODE
// EXISTING_CODE
