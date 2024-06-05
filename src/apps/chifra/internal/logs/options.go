// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package logsPkg

import (
	// EXISTING_CODE
	"encoding/json"
	"io"
	"net/http"
	"net/url"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
	// EXISTING_CODE
)

// LogsOptions provides all command options for the chifra logs command.
type LogsOptions struct {
	Transactions   []string                 `json:"transactions,omitempty"`   // A space-separated list of one or more transaction identifiers
	TransactionIds []identifiers.Identifier `json:"transactionIds,omitempty"` // Transaction identifiers
	Emitter        []string                 `json:"emitter,omitempty"`        // Filter logs to show only those logs emitted by the given address(es)
	Topic          []string                 `json:"topic,omitempty"`          // Filter logs to show only those with this topic(s)
	Articulate     bool                     `json:"articulate,omitempty"`     // Articulate the retrieved data if ABIs can be found
	Globals        globals.GlobalOptions    `json:"globals,omitempty"`        // The global options
	Conn           *rpc.Connection          `json:"conn,omitempty"`           // The connection to the RPC server
	BadFlag        error                    `json:"badFlag,omitempty"`        // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultLogsOptions = LogsOptions{}

// testLog is used only during testing to export the options for this test case.
func (opts *LogsOptions) testLog() {
	logger.TestLog(len(opts.Transactions) > 0, "Transactions: ", opts.Transactions)
	logger.TestLog(len(opts.Emitter) > 0, "Emitter: ", opts.Emitter)
	logger.TestLog(len(opts.Topic) > 0, "Topic: ", opts.Topic)
	logger.TestLog(opts.Articulate, "Articulate: ", opts.Articulate)
	opts.Conn.TestLog(opts.getCaches())
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *LogsOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// logsFinishParseApi finishes the parsing for server invocations. Returns a new LogsOptions.
func logsFinishParseApi(w http.ResponseWriter, r *http.Request) *LogsOptions {
	values := r.URL.Query()
	if r.Header.Get("User-Agent") == "testRunner" {
		values.Set("testRunner", "true")
	}
	return LogsFinishParseInternal(w, values)
}

func LogsFinishParseInternal(w io.Writer, values url.Values) *LogsOptions {
	copy := defaultLogsOptions
	copy.Globals.Caps = getCaps()
	opts := &copy
	for key, value := range values {
		switch key {
		case "transactions":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Transactions = append(opts.Transactions, s...)
			}
		case "emitter":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Emitter = append(opts.Emitter, s...)
			}
		case "topic":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Topic = append(opts.Topic, s...)
			}
		case "articulate":
			opts.Articulate = true
		default:
			if !copy.Globals.Caps.HasKey(key) {
				err := validate.Usage("Invalid key ({0}) in {1} route.", key, "logs")
				if opts.BadFlag == nil || opts.BadFlag.Error() > err.Error() {
					opts.BadFlag = err
				}
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, values, opts.getCaches())

	// EXISTING_CODE
	// EXISTING_CODE
	opts.Emitter, _ = opts.Conn.GetEnsAddresses(opts.Emitter)

	return opts
}

// logsFinishParse finishes the parsing for command line invocations. Returns a new LogsOptions.
func logsFinishParse(args []string) *LogsOptions {
	// remove duplicates from args if any (not needed in api mode because the server does it).
	dedup := map[string]int{}
	if len(args) > 0 {
		tmp := []string{}
		for _, arg := range args {
			if value := dedup[arg]; value == 0 {
				tmp = append(tmp, arg)
			}
			dedup[arg]++
		}
		args = tmp
	}

	defFmt := "txt"
	opts := GetOptions()
	opts.Conn = opts.Globals.FinishParse(args, opts.getCaches())

	// EXISTING_CODE
	opts.Transactions = args
	// EXISTING_CODE
	opts.Emitter, _ = opts.Conn.GetEnsAddresses(opts.Emitter)
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}

	return opts
}

func GetOptions() *LogsOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultLogsOptions
}

func getCaps() caps.Capability {
	var capabilities caps.Capability // capabilities for chifra logs
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Caching)
	// EXISTING_CODE
	// EXISTING_CODE
	return capabilities
}

func ResetOptions(testMode bool) {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	opts := LogsOptions{}
	globals.SetDefaults(&opts.Globals)
	opts.Globals.TestMode = testMode
	opts.Globals.Writer = w
	opts.Globals.Caps = getCaps()
	defaultLogsOptions = opts
}

func (opts *LogsOptions) getCaches() (caches map[walk.CacheType]bool) {
	// EXISTING_CODE
	caches = map[walk.CacheType]bool{
		walk.Cache_Transactions: true,
	}
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
