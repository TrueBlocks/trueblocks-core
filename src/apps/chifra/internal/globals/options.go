// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package globals

import (
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/spf13/cobra"
)

type GlobalOptions struct {
	Wei     bool   `json:"wei,omitempty"`
	Ether   bool   `json:"ether,omitempty"`
	Help    bool   `json:"help,omitempty"`
	File    string `json:"file,omitempty"`
	Version bool   `json:"version,omitempty"`
	Noop    bool   `json:"noop,omitempty"`
	NoColor bool   `json:"noColor,omitempty"`
	output.OutputOptions
}

func (opts *GlobalOptions) TestLog() {
	logger.TestLog(opts.Verbose, "Verbose: ", opts.Verbose)
	logger.TestLog(opts.LogLevel > 0, "LogLevel: ", opts.LogLevel)
	logger.TestLog(opts.NoHeader, "NoHeader: ", opts.NoHeader)
	logger.TestLog(len(opts.Chain) > 0 && opts.Chain != config.GetDefaultChain(), "Chain: ", opts.Chain)
	logger.TestLog(opts.Wei, "Wei: ", opts.Wei)
	logger.TestLog(opts.Ether, "Ether: ", opts.Ether)
	logger.TestLog(opts.Help, "Help: ", opts.Help)
	logger.TestLog(opts.ShowRaw, "ShowRaw: ", opts.ShowRaw)
	logger.TestLog(opts.ToFile, "ToFile: ", opts.ToFile)
	logger.TestLog(len(opts.File) > 0, "File: ", opts.File)
	logger.TestLog(opts.Version, "Version: ", opts.Version)
	logger.TestLog(opts.Noop, "Noop: ", opts.Noop)
	logger.TestLog(opts.NoColor, "NoColor: ", opts.NoColor)
	logger.TestLog(len(opts.OutputFn) > 0, "OutputFn: ", opts.OutputFn)
	logger.TestLog(opts.Append, "Append: ", opts.Append)
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

func InitGlobals(cmd *cobra.Command, opts *GlobalOptions) {
	opts.TestMode = file.IsTestMode()

	cmd.Flags().StringVarP(&opts.Format, "fmt", "x", "", "export format, one of [none|json*|txt|csv]")
	cmd.Flags().BoolVarP(&opts.Verbose, "verbose", "v", false, "enable verbose (increase detail with --log_level)")
	cmd.Flags().BoolVarP(&opts.Help, "help", "h", false, "display this help screen")

	cmd.Flags().StringVarP(&opts.Chain, "chain", "", "", "EVM compatible chain you're running against")
	cmd.Flags().BoolVarP(&opts.ShowRaw, "raw", "", false, "report JSON data from the node with minimal processing")
	cmd.Flags().BoolVarP(&opts.Version, "version", "", false, "display the current version of the tool")
	cmd.Flags().BoolVarP(&opts.Noop, "noop", "", false, "")
	cmd.Flags().BoolVarP(&opts.NoColor, "nocolor", "", false, "")
	cmd.Flags().Uint64VarP(&opts.LogLevel, "log_level", "", 0, "")
	cmd.Flags().BoolVarP(&opts.NoHeader, "no_header", "", false, "supress export of header row for csv and txt exports")
	cmd.Flags().BoolVarP(&opts.Wei, "wei", "", false, "specify value in wei (the default)")
	cmd.Flags().BoolVarP(&opts.Ether, "ether", "", false, "specify value in ether")
	cmd.Flags().StringVarP(&opts.File, "file", "", "", "specify multiple command line options in a file")
	cmd.Flags().BoolVarP(&opts.ToFile, "to_file", "", false, "write the results to a temporary file and return the filename")
	cmd.Flags().StringVarP(&opts.OutputFn, "output", "", "", "redirect results from stdout to the given file, create if not present")
	cmd.Flags().BoolVarP(&opts.Append, "append", "", false, "if true, open OutputFn for append (truncate otherwise)")

	cmd.Flags().MarkHidden("chain")
	cmd.Flags().MarkHidden("raw")
	cmd.Flags().MarkHidden("version")
	cmd.Flags().MarkHidden("noop")
	cmd.Flags().MarkHidden("nocolor")
	cmd.Flags().MarkHidden("log_level")
	cmd.Flags().MarkHidden("no_header")
	cmd.Flags().MarkHidden("wei")
	cmd.Flags().MarkHidden("ether")
	cmd.Flags().MarkHidden("to_file")
	cmd.Flags().MarkHidden("file")
	cmd.Flags().MarkHidden("output")
	cmd.Flags().MarkHidden("append")

	SetDefaults(opts)
}

func (opts *GlobalOptions) toCmdLine() string {
	options := ""
	if opts.ShowRaw {
		options += " --raw"
	}
	if opts.Version {
		options += " --version"
	}
	if len(opts.Format) > 0 {
		options += " --fmt " + opts.Format
	}
	if opts.Verbose || opts.LogLevel > 0 {
		level := opts.LogLevel
		if level == 0 {
			level = 1
		}
		options += " --verbose " + fmt.Sprintf("%d", level)
	}
	if len(opts.OutputFn) > 0 {
		options += " --output " + opts.OutputFn
	}
	if opts.Append {
		options += " --append"
	}
	if opts.NoHeader {
		options += " --no_header"
	}
	if opts.Wei {
		options += " --wei"
	}
	if opts.Ether {
		options += " --ether"
	}
	if opts.ToFile {
		options += " --to_file"
	}

	return options
}

func GlobalsFinishParseApi(w http.ResponseWriter, r *http.Request) *GlobalOptions {
	opts := &GlobalOptions{}
	opts.TestMode = r.Header.Get("User-Agent") == "testRunner"
	opts.Writer = w

	for key, value := range r.URL.Query() {
		switch key {
		case "fmt":
			opts.Format = value[0]
		case "verbose":
			opts.Verbose = true
		case "raw":
			opts.ShowRaw = true
		case "version":
			opts.Version = true
		case "noop":
			// do nothing
		case "nocolor":
			opts.NoColor = true
		case "logLevel":
			opts.LogLevel = ToUint64(value[0])
		case "noHeader":
			opts.NoHeader = true
		case "chain":
			opts.Chain = value[0]
		case "wei":
			opts.Wei = true
		case "ether":
			opts.Ether = true
		case "toFile":
			opts.ToFile = true
		case "file":
			opts.File = value[0]
		case "output":
			opts.OutputFn = value[0]
		case "append":
			opts.Append = true
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
		fmt.Println("Could not establish ts file:", err)
	}

	return opts

	// The 'help' command is a special case for cobra, so doesn't need to be handled here
	// cmd.Flags().BoolVarP(&opts.Help, "help", "h", false, "display this help screen")
}

func (opts *GlobalOptions) FinishParse(args []string) {
	if err := tslib.EstablishTsFile(opts.Chain); err != nil {
		fmt.Println("Could not establish ts file:", err)
	}
}

func IsGlobalOption(key string) bool {
	permitted := []string{
		"fmt",
		"verbose",
		"raw",
		"version",
		"noop",
		"nocolor",
		"logLevel",
		"noHeader",
		"chain",
		"wei",
		"ether",
		"toFile",
		"file",
		"output",
		"append",
	}
	for _, per := range permitted {
		if per == key {
			return true
		}
	}
	return false
}
