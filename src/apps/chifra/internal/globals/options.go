// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package globals

import (
	"io"
	"net/url"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
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
	// Names is internal use only
	cc := opts.Caps.Remove(caps.Names)

	logger.TestLog(opts.Verbose, "Verbose: ", opts.Verbose)
	logger.TestLog(opts.NoHeader, "NoHeader: ", opts.NoHeader)
	logger.TestLog(len(opts.Chain) > 0 && opts.Chain != config.GetSettings().DefaultChain, "Chain: ", opts.Chain)
	logger.TestLog(opts.Wei, "Wei: ", opts.Wei)
	logger.TestLog(opts.Ether, "Ether: ", opts.Ether)
	logger.TestLog(opts.Help, "Help: ", opts.Help)
	logger.TestLog(len(opts.File) > 0, "File: ", opts.File)
	logger.TestLog(opts.Version, "Version: ", opts.Version)
	logger.TestLog(opts.Noop, "Noop: ", opts.Noop)
	logger.TestLog(opts.NoColor, "NoColor: ", opts.NoColor)
	logger.TestLog(len(opts.OutputFn) > 0, "OutputFn: ", opts.OutputFn)
	logger.TestLog(opts.Append, "Append: ", opts.Append)
	logger.TestLog(opts.Cache, "Cache: ", opts.Cache)
	logger.TestLog(opts.Decache, "Decache: ", opts.Decache)
	logger.TestLog(cc != caps.Default, "Caps: ", opts.Caps.Show())
	logger.TestLog(len(opts.Format) > 0, "Format: ", opts.Format)
	// logger.TestLog(opts.TestMode, "TestMode: ", opts.TestMode)
}

func SetDefaults(opts *GlobalOptions) {
	if len(opts.Chain) == 0 {
		opts.Chain = config.GetSettings().DefaultChain
	}
}

func (opts *GlobalOptions) ShouldLoadNames(force bool) bool {
	return opts.Caps.Has(caps.Names) && (force || opts.Verbose)
}

// TODO: These options should be in a data file
func InitGlobals(whoAmI string, cmd *cobra.Command, opts *GlobalOptions, c caps.Capability) {
	opts.TestMode = base.IsTestMode()
	opts.Caps = c

	if opts.Caps.Has(caps.Ether) {
		cmd.Flags().BoolVarP(&opts.Ether, "ether", "H", false, "specify value in ether")
	}

	if opts.Caps.Has(caps.Wei) {
		cmd.Flags().BoolVarP(&opts.Wei, "wei", "W", false, "specify value in wei (the default)")
	}
	_ = cmd.Flags().MarkHidden("wei")

	if opts.Caps.Has(caps.Caching) {
		if whoAmI != "monitors" {
			cmd.Flags().BoolVarP(&opts.Cache, "cache", "o", false, "force the results of the query into the cache")
		}
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
		cmd.Flags().BoolVarP(&opts.NoHeader, "no_header", "", false, "suppress export of header row for csv and txt exports")
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

func (opts *GlobalOptions) FinishParseApi(w io.Writer, values url.Values, caches map[walk.CacheType]bool) *rpc.Connection {
	opts.Writer = w

	for key, value := range values {
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
		case "verbose":
			opts.Verbose = true
		case "version":
			opts.Version = true
		case "wei":
			opts.Wei = true
		case "testRunner":
			opts.TestMode = true
			colors.ColorsOff()
		}
	}

	if len(opts.Format) == 0 || opts.Format == "none" {
		opts.Format = "json"
		if len(opts.OutputFn) > 0 {
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
		opts.Chain = config.GetSettings().DefaultChain
	}

	if config.IsChainConfigured(opts.Chain) {
		conn := rpc.NewConnection(opts.Chain, opts.Cache, caches)
		publisher, _ := conn.GetEnsAddress(config.GetPublisher(""))
		publisherAddr := base.HexToAddress(publisher)
		if err := tslib.EstablishTimestamps(opts.Chain, publisherAddr); err != nil {
			logger.Warn(err)
		}
		return conn
	} else {
		// the error will be reported by the validator
		return rpc.TempConnection(opts.Chain)
	}
}

func (opts *GlobalOptions) FinishParse(args []string, caches map[walk.CacheType]bool) *rpc.Connection {
	_ = args
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
		opts.Chain = config.GetSettings().DefaultChain
	}

	if config.IsChainConfigured(opts.Chain) {
		conn := rpc.NewConnection(opts.Chain, opts.Cache, caches)
		publisher, _ := conn.GetEnsAddress(config.GetPublisher(""))
		publisherAddr := base.HexToAddress(publisher)
		if err := tslib.EstablishTimestamps(opts.Chain, publisherAddr); err != nil {
			logger.Warn(err)
		}
		return conn
	} else {
		// the error will be reported by the validator
		return rpc.TempConnection(opts.Chain)
	}
}
