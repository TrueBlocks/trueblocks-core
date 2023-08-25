// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package globals

import (
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/spf13/cobra"
)

type GlobalOptions struct {
	Wei     bool            `json:"wei,omitempty"`
	Ether   bool            `json:"ether,omitempty"`
	Help    bool            `json:"help,omitempty"`
	File    string          `json:"file,omitempty"`
	Version bool            `json:"version,omitempty"`
	Noop    bool            `json:"noop,omitempty"`
	NoColor bool            `json:"noColor,omitempty"`
	Cache   bool            `json:"cache,omitempty"`
	Decache bool            `json:"decache,omitempty"`
	Caps    caps.Capability `json:"-"`
	output.OutputOptions
}

func (opts *GlobalOptions) TestLog() {
	logger.TestLog(opts.Verbose, "Verbose: ", opts.Verbose)
	logger.TestLog(opts.NoHeader, "NoHeader: ", opts.NoHeader)
	logger.TestLog(len(opts.Chain) > 0 && opts.Chain != config.GetDefaultChain(), "Chain: ", opts.Chain)
	logger.TestLog(opts.Wei, "Wei: ", opts.Wei)
	logger.TestLog(opts.Ether, "Ether: ", opts.Ether)
	logger.TestLog(opts.Help, "Help: ", opts.Help)
	logger.TestLog(opts.ShowRaw, "ShowRaw: ", opts.ShowRaw)
	logger.TestLog(len(opts.File) > 0, "File: ", opts.File)
	logger.TestLog(opts.Version, "Version: ", opts.Version)
	logger.TestLog(opts.Noop, "Noop: ", opts.Noop)
	logger.TestLog(opts.NoColor, "NoColor: ", opts.NoColor)
	logger.TestLog(len(opts.OutputFn) > 0, "OutputFn: ", opts.OutputFn)
	logger.TestLog(opts.Append, "Append: ", opts.Append)
	logger.TestLog(opts.Cache, "Cache: ", opts.Cache)
	logger.TestLog(opts.Decache, "Decache: ", opts.Decache)
	logger.TestLog(opts.Caps != caps.Default, "Caps: ", opts.Caps.Show())
	logger.TestLog(len(opts.Format) > 0, "Format: ", opts.Format)
	// logger.TestLog(opts.TestMode, "TestMode: ", opts.TestMode)
}

func SetDefaults(opts *GlobalOptions) {
	if len(opts.Chain) == 0 {
		opts.Chain = config.GetDefaultChain()
	}

	if opts.ShowRaw {
		opts.Format = "json"
	}
}

// TODO: These options should be in a data file
func InitGlobals(cmd *cobra.Command, opts *GlobalOptions, c caps.Capability) {
	opts.TestMode = file.IsTestMode()
	opts.Caps = c

	if opts.Caps.Has(caps.Ether) {
		cmd.Flags().BoolVarP(&opts.Ether, "ether", "H", false, "specify value in ether")
	}

	if opts.Caps.Has(caps.Wei) {
		cmd.Flags().BoolVarP(&opts.Wei, "wei", "W", false, "specify value in wei (the default)")
	}
	_ = cmd.Flags().MarkHidden("wei")

	if opts.Caps.Has(caps.Raw) {
		cmd.Flags().BoolVarP(&opts.ShowRaw, "raw", "w", false, "report JSON data from the source with minimal processing")
	}

	if opts.Caps.Has(caps.Caching) {
		cmd.Flags().BoolVarP(&opts.Cache, "cache", "o", false, "force the results of the query into the cache")
		cmd.Flags().BoolVarP(&opts.Decache, "decache", "D", false, "removes related items from the cache")
	}

	if opts.Caps.Has(caps.Fmt) {
		cmd.Flags().StringVarP(&opts.Format, "fmt", "x", "", "export format, one of [none|json*|txt|csv]")
	}

	if opts.Caps.Has(caps.Verbose) {
		cmd.Flags().BoolVarP(&opts.Verbose, "verbose", "v", false, "enable verbose output")
	}

	cmd.Flags().BoolVarP(&opts.Help, "help", "h", false, "display this help screen")

	if opts.Caps.Has(caps.Chain) {
		cmd.Flags().StringVarP(&opts.Chain, "chain", "", "", "EVM compatible chain you're running against")
	}
	_ = cmd.Flags().MarkHidden("chain")

	if opts.Caps.Has(caps.Version) {
		cmd.Flags().BoolVarP(&opts.Version, "version", "", false, "display the current version of the tool")
	}
	_ = cmd.Flags().MarkHidden("version")

	if opts.Caps.Has(caps.Noop) {
		cmd.Flags().BoolVarP(&opts.Noop, "noop", "", false, "")
	}
	_ = cmd.Flags().MarkHidden("noop")

	if opts.Caps.Has(caps.NoColor) {
		cmd.Flags().BoolVarP(&opts.NoColor, "nocolor", "", false, "")
	}
	_ = cmd.Flags().MarkHidden("nocolor")

	if opts.Caps.Has(caps.NoHeader) {
		cmd.Flags().BoolVarP(&opts.NoHeader, "no_header", "", false, "supress export of header row for csv and txt exports")
	}
	_ = cmd.Flags().MarkHidden("no_header")

	// if opts.Caps.Has(caps.File) {
	cmd.Flags().StringVarP(&opts.File, "file", "", "", "specify multiple command line options in a file")
	// }
	_ = cmd.Flags().MarkHidden("file")

	if opts.Caps.Has(caps.Output) {
		cmd.Flags().StringVarP(&opts.OutputFn, "output", "", "", "redirect results from stdout to the given file, create if not present")
	}
	_ = cmd.Flags().MarkHidden("output")

	if opts.Caps.Has(caps.Append) {
		cmd.Flags().BoolVarP(&opts.Append, "append", "", false, "if true, open OutputFn for append (truncate otherwise)")
	}
	_ = cmd.Flags().MarkHidden("append")

	SetDefaults(opts)
}

func (opts *GlobalOptions) FinishParseApi(w http.ResponseWriter, r *http.Request, caches map[string]bool) *rpc.Connection {
	opts.TestMode = r.Header.Get("User-Agent") == "testRunner"
	opts.Writer = w

	for key, value := range r.URL.Query() {
		switch key {
		case "append":
			opts.Append = true
		case "cache":
			opts.Cache = true
		case "chain":
			opts.Chain = value[0]
		case "decache":
			opts.Decache = true
		case "ether":
			opts.Ether = true
		case "file":
			opts.File = value[0]
		case "fmt":
			opts.Format = value[0]
		case "nocolor":
			opts.NoColor = true
		case "noHeader":
			opts.NoHeader = true
		case "noop":
			// do nothing
		case "output":
			opts.OutputFn = value[0]
		case "raw":
			opts.ShowRaw = true
		case "verbose":
			opts.Verbose = true
		case "version":
			opts.Version = true
		case "wei":
			opts.Wei = true
		}
	}

	if len(opts.Format) == 0 || opts.Format == "none" || opts.ShowRaw {
		opts.Format = "json"
		if !opts.ShowRaw && len(opts.OutputFn) > 0 {
			parts := strings.Split(opts.OutputFn, ".")
			if len(parts) > 0 {
				last := parts[len(parts)-1]
				if last == "txt" || last == "csv" || last == "json" {
					opts.Format = last
				}
			}
		}
	}

	if len(opts.Chain) == 0 {
		opts.Chain = config.GetDefaultChain()
	}

	if err := tslib.EstablishTsFile(opts.Chain); err != nil {
		logger.Error("Could not establish ts file:", err)
	}

	return rpc.NewConnection(opts.Chain, opts.Cache && !opts.ShowRaw, caches)
}

func (opts *GlobalOptions) FinishParse(args []string, caches map[string]bool) *rpc.Connection {
	if (len(opts.Format) == 0 || opts.Format == "none") && len(opts.OutputFn) > 0 {
		parts := strings.Split(opts.OutputFn, ".")
		if len(parts) > 0 {
			last := parts[len(parts)-1]
			if last == "txt" || last == "csv" || last == "json" {
				opts.Format = last
			}
		}
	}

	if len(opts.Chain) == 0 {
		opts.Chain = config.GetDefaultChain()
	}

	if err := tslib.EstablishTsFile(opts.Chain); err != nil {
		logger.Error("Could not establish ts file:", err)
	}

	return rpc.NewConnection(opts.Chain, opts.Cache && !opts.ShowRaw, caches)
}
