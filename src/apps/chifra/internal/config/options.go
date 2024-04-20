// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package configPkg

import (
	// EXISTING_CODE
	"encoding/json"
	"io"
	"net/http"
	"net/url"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	// EXISTING_CODE
)

// ConfigOptions provides all command options for the chifra config command.
type ConfigOptions struct {
	Mode    string                `json:"mode,omitempty"`    // Either show or edit the configuration
	Paths   bool                  `json:"paths,omitempty"`   // Show the configuration paths for the system
	Globals globals.GlobalOptions `json:"globals,omitempty"` // The global options
	Conn    *rpc.Connection       `json:"conn,omitempty"`    // The connection to the RPC server
	BadFlag error                 `json:"badFlag,omitempty"` // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultConfigOptions = ConfigOptions{}

// testLog is used only during testing to export the options for this test case.
func (opts *ConfigOptions) testLog() {
	logger.TestLog(len(opts.Mode) > 0, "Mode: ", opts.Mode)
	logger.TestLog(opts.Paths, "Paths: ", opts.Paths)
	opts.Conn.TestLog(opts.getCaches())
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *ConfigOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// configFinishParseApi finishes the parsing for server invocations. Returns a new ConfigOptions.
func configFinishParseApi(w http.ResponseWriter, r *http.Request) *ConfigOptions {
	values := r.URL.Query()
	if r.Header.Get("User-Agent") == "testRunner" {
		values.Set("testRunner", "true")
	}
	return ConfigFinishParseInternal(w, values)
}

func ConfigFinishParseInternal(w io.Writer, values url.Values) *ConfigOptions {
	copy := defaultConfigOptions
	opts := &copy
	for key, value := range values {
		switch key {
		case "mode":
			opts.Mode = value[0]
		case "paths":
			opts.Paths = true
		default:
			if !copy.Globals.Caps.HasKey(key) {
				err := validate.Usage("Invalid key ({0}) in {1} route.", key, "config")
				if opts.BadFlag == nil || opts.BadFlag.Error() > err.Error() {
					opts.BadFlag = err
				}
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, values, opts.getCaches())

	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

// configFinishParse finishes the parsing for command line invocations. Returns a new ConfigOptions.
func configFinishParse(args []string) *ConfigOptions {
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
	defFmt = ""
	for _, arg := range args {
		if arg == "show" || arg == "edit" {
			opts.Mode = arg
		}
	}
	if len(opts.Mode) == 0 {
		opts.Mode = "show"
	}
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}

	return opts
}

func GetOptions() *ConfigOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultConfigOptions
}

func ResetOptions(testMode bool) {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultConfigOptions = ConfigOptions{}
	globals.SetDefaults(&defaultConfigOptions.Globals)
	defaultConfigOptions.Globals.TestMode = testMode
	defaultConfigOptions.Globals.Writer = w
	var capabilities caps.Capability // capabilities for chifra config
	capabilities = capabilities.Add(caps.Default)
	// EXISTING_CODE
	// EXISTING_CODE
	defaultConfigOptions.Globals.Caps = capabilities
}

func (opts *ConfigOptions) getCaches() (m map[string]bool) {
	// EXISTING_CODE
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
