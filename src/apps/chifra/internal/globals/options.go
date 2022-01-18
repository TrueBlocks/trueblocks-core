// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package globals

import (
	"fmt"
	"net/http"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/spf13/cobra"
)

type GlobalOptions struct {
	Verbose  bool
	LogLevel uint64
	NoHeader bool
	Chain    string
	Wei      bool
	Ether    bool
	Dollars  bool
	Help     bool
	Raw      bool
	ToFile   bool
	File     string
	Version  bool
	Noop     bool
	Mocked   bool
	NoColor  bool
	OutputFn string
	Format   string
	TestMode bool
	ApiMode  bool
	Writer   http.ResponseWriter
}

func (opts *GlobalOptions) TestLog() {
	logger.TestLog(opts.Verbose, "Verbose: ", opts.Verbose)
	logger.TestLog(opts.LogLevel > 0, "LogLevel: ", opts.LogLevel)
	logger.TestLog(opts.NoHeader, "NoHeader: ", opts.NoHeader)
	logger.TestLog(len(opts.Chain) > 0 && opts.Chain != config.GetDefaultChain(), "Chain: ", opts.Chain)
	logger.TestLog(opts.Wei, "Wei: ", opts.Wei)
	logger.TestLog(opts.Ether, "Ether: ", opts.Ether)
	logger.TestLog(opts.Dollars, "Dollars: ", opts.Dollars)
	logger.TestLog(opts.Help, "Help: ", opts.Help)
	logger.TestLog(opts.Raw, "Raw: ", opts.Raw)
	logger.TestLog(opts.ToFile, "ToFile: ", opts.ToFile)
	logger.TestLog(len(opts.File) > 0, "File: ", opts.File)
	logger.TestLog(opts.Version, "Version: ", opts.Version)
	logger.TestLog(opts.Noop, "Noop: ", opts.Noop)
	logger.TestLog(opts.Mocked, "Mocked: ", opts.Mocked)
	logger.TestLog(opts.NoColor, "NoColor: ", opts.NoColor)
	logger.TestLog(len(opts.OutputFn) > 0, "OutputFn: ", opts.OutputFn)
	logger.TestLog(len(opts.Format) > 0, "Format: ", opts.Format)
	// logger.TestLog(opts.TestMode, "TestMode: ", opts.TestMode)
	logger.TestLog(opts.ApiMode, "ApiMode: ", opts.ApiMode)
}

func InitGlobals(cmd *cobra.Command, opts *GlobalOptions) {
	opts.TestMode = os.Getenv("TEST_MODE") == "true"
	opts.ApiMode = os.Getenv("API_MODE") == "true"

	cmd.Flags().StringVarP(&opts.Format, "fmt", "x", "", "export format, one of [none|json*|txt|csv|api]")
	cmd.Flags().BoolVarP(&opts.Verbose, "verbose", "v", false, "enable verbose (increase detail with --log_level)")
	cmd.Flags().BoolVarP(&opts.Help, "help", "h", false, "display this help screen")

	// TODO: BOGUS
	cmd.Flags().StringVarP(&opts.Chain, "chain", "", "", "EVM compatible chain you're running against")
	cmd.Flags().BoolVarP(&opts.Raw, "raw", "", false, "report JSON data from the node with minimal processing")
	cmd.Flags().BoolVarP(&opts.Version, "version", "", false, "display the current version of the tool")
	cmd.Flags().BoolVarP(&opts.Noop, "noop", "", false, "")
	cmd.Flags().BoolVarP(&opts.Mocked, "mocked", "", false, "")
	cmd.Flags().BoolVarP(&opts.NoColor, "nocolor", "", false, "")
	cmd.Flags().Uint64VarP(&opts.LogLevel, "log_level", "", 0, "")
	cmd.Flags().BoolVarP(&opts.NoHeader, "no_header", "", false, "supress export of header row for csv and txt exports")
	cmd.Flags().BoolVarP(&opts.Wei, "wei", "", false, "specify value in wei (the default)")
	cmd.Flags().BoolVarP(&opts.Ether, "ether", "", false, "specify value in ether")
	cmd.Flags().BoolVarP(&opts.Dollars, "dollars", "", false, "specify value in US dollars")
	cmd.Flags().BoolVarP(&opts.ToFile, "to_file", "", false, "write the results to a temporary file and return the filename")
	cmd.Flags().StringVarP(&opts.File, "file", "", "", "specify multiple sets of command line options in a file")
	cmd.Flags().StringVarP(&opts.OutputFn, "output", "", "", "write the results to file 'fn' and return the filename")

	cmd.Flags().MarkHidden("chain")
	cmd.Flags().MarkHidden("raw")
	cmd.Flags().MarkHidden("version")
	cmd.Flags().MarkHidden("noop")
	cmd.Flags().MarkHidden("mocked")
	cmd.Flags().MarkHidden("nocolor")
	cmd.Flags().MarkHidden("log_level")
	cmd.Flags().MarkHidden("no_header")
	cmd.Flags().MarkHidden("wei")
	cmd.Flags().MarkHidden("ether")
	cmd.Flags().MarkHidden("dollars")
	cmd.Flags().MarkHidden("to_file")
	cmd.Flags().MarkHidden("file")
	cmd.Flags().MarkHidden("output")

	if len(opts.Chain) == 0 {
		opts.Chain = config.GetDefaultChain()
	}
}

func (opts *GlobalOptions) ToCmdLine() string {
	options := ""
	if opts.Raw {
		options += " --raw"
	}
	// if opts.Noop {
	// 	options += " --noop"
	// }
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
	if opts.NoHeader {
		options += " --no_header"
	}
	if len(opts.Chain) > 0 {
		// TODO: Do we need --chain in the c++ code?
		// fmt.Fprintf(os.Stderr, "chain: %s\n", opts.Chain)
		// options += " --chain" + opts.Chain
	}
	if opts.Wei {
		options += " --wei"
	}
	if opts.Ether {
		options += " --ether"
	}
	if opts.Dollars {
		options += " --dollars"
	}
	if opts.ToFile {
		options += " --to_file"
	}
	if len(opts.File) > 0 {
		// TODO: one of the problems with this is that if the file contains invalid commands,
		// TODO: because we don't see those commands until we're doing into the tool, we
		// TODO: can't report on the 'bad command' in Cobra format. This will require us to
		// TODO: keep validation code down in the tools which we want to avoid. To fix this
		// TODO: the code below should open the file, read each command, and recursively call
		// TODO: into chifra here.
		options += " --file:" + opts.File
	}
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *GlobalOptions {
	opts := &GlobalOptions{}
	opts.Writer = w

	opts.TestMode = r.Header.Get("User-Agent") == "testRunner"
	opts.ApiMode = true

	for key, value := range r.URL.Query() {
		switch key {
		case "fmt":
			opts.Format = value[0]
		case "verbose":
			opts.Verbose = true
		case "raw":
			opts.Raw = true
		case "version":
			opts.Version = true
		case "noop":
			// do nothing
		case "mocked":
			opts.Mocked = true
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
		case "dollars":
			opts.Dollars = true
		case "toFile":
			opts.ToFile = true
		case "file":
			opts.File = value[0]
		case "output":
			opts.OutputFn = value[0]
		}
	}
	if len(opts.Chain) == 0 {
		opts.Chain = config.GetDefaultChain()
	}
	return opts

	// The 'help' command is a special case for cobra, so doesn't need to be handled here
	// cmd.Flags().BoolVarP(&opts.Help, "help", "h", false, "display this help screen")
}

func IsGlobalOption(key string) bool {
	permitted := []string{
		"fmt",
		"verbose",
		"raw",
		"version",
		"noop",
		"mocked",
		"nocolor",
		"logLevel",
		"noHeader",
		"chain",
		"wei",
		"ether",
		"dollars",
		"toFile",
		"file",
		"output",
	}
	for _, per := range permitted {
		if per == key {
			return true
		}
	}
	return false
}
