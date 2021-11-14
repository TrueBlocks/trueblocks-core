package globals

/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/

import (
	"net/http"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/spf13/cobra"
)

type GlobalOptionsType struct {
	Verbose  bool
	LogLevel uint64
	NoHeader bool
	Wei      bool
	Ether    bool
	Dollars  bool
	Help     bool
	Raw      bool
	ToFile   bool
	File     string
	Version  bool
	Noop     bool
	OutputFn string
	Format   string
	TestMode bool
	ApiMode  bool
	Writer   http.ResponseWriter
}

func (opts *GlobalOptionsType) TestLog() {
	logger.TestLog(opts.Verbose, "Verbose: ", opts.Verbose)
	logger.TestLog(opts.LogLevel > 0, "LogLevel: ", opts.LogLevel)
	logger.TestLog(opts.NoHeader, "NoHeader: ", opts.NoHeader)
	logger.TestLog(opts.Wei, "Wei: ", opts.Wei)
	logger.TestLog(opts.Ether, "Ether: ", opts.Ether)
	logger.TestLog(opts.Dollars, "Dollars: ", opts.Dollars)
	logger.TestLog(opts.Help, "Help: ", opts.Help)
	logger.TestLog(opts.Raw, "Raw: ", opts.Raw)
	logger.TestLog(opts.ToFile, "ToFile: ", opts.ToFile)
	logger.TestLog(len(opts.File) > 0, "File: ", opts.File)
	logger.TestLog(opts.Version, "Version: ", opts.Version)
	logger.TestLog(opts.Noop, "Noop: ", opts.Noop)
	logger.TestLog(len(opts.OutputFn) > 0, "OutputFn: ", opts.OutputFn)
	logger.TestLog(len(opts.Format) > 0, "Format: ", opts.Format)
	// logger.TestLog(opts.TestMode, "TestMode: ", opts.TestMode)
	logger.TestLog(opts.ApiMode, "ApiMode: ", opts.ApiMode)
}

func InitGlobals(cmd *cobra.Command, opts *GlobalOptionsType) {
	opts.TestMode = os.Getenv("TEST_MODE") == "true"
	opts.ApiMode = os.Getenv("API_MODE") == "true"

	cmd.Flags().StringVarP(&opts.Format, "fmt", "x", "", "export format, one of [none|json*|txt|csv|api]")
	cmd.Flags().BoolVarP(&opts.Verbose, "verbose", "v", false, "enable verbose (increase detail with --log_level)")
	cmd.Flags().BoolVarP(&opts.Help, "help", "h", false, "display this help screen")

	cmd.Flags().BoolVarP(&opts.Raw, "raw", "", false, "report JSON data from the node with minimal processing")
	cmd.Flags().BoolVarP(&opts.Version, "version", "", false, "display the current version of the tool")
	cmd.Flags().BoolVarP(&opts.Noop, "noop", "", false, "")
	cmd.Flags().Uint64VarP(&opts.LogLevel, "log_level", "", 0, "")
	cmd.Flags().BoolVarP(&opts.NoHeader, "no_header", "", false, "supress export of header row for csv and txt exports")
	cmd.Flags().BoolVarP(&opts.Wei, "wei", "", false, "specify value in wei (the default)")
	cmd.Flags().BoolVarP(&opts.Ether, "ether", "", false, "specify value in ether")
	cmd.Flags().BoolVarP(&opts.Dollars, "dollars", "", false, "specify value in US dollars")
	cmd.Flags().BoolVarP(&opts.ToFile, "to_file", "", false, "write the results to a temporary file and return the filename")
	cmd.Flags().StringVarP(&opts.File, "file", "", "", "specify multiple sets of command line options in a file")
	cmd.Flags().StringVarP(&opts.OutputFn, "output", "", "", "write the results to file 'fn' and return the filename")

	cmd.Flags().MarkHidden("raw")
	cmd.Flags().MarkHidden("version")
	cmd.Flags().MarkHidden("noop")
	cmd.Flags().MarkHidden("log_level")
	cmd.Flags().MarkHidden("no_header")
	cmd.Flags().MarkHidden("wei")
	cmd.Flags().MarkHidden("ether")
	cmd.Flags().MarkHidden("dollars")
	cmd.Flags().MarkHidden("to_file")
	cmd.Flags().MarkHidden("file")
	cmd.Flags().MarkHidden("output")
}

func FromRequest(w http.ResponseWriter, r *http.Request) *GlobalOptionsType {
	opts := &GlobalOptionsType{}
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
		case "log_level":
			opts.LogLevel = ToUint64(value[0])
		case "no_header":
			opts.NoHeader = true
		case "wei":
			opts.Wei = true
		case "ether":
			opts.Ether = true
		case "dollars":
			opts.Dollars = true
		case "to_file":
			opts.ToFile = true
		case "file":
			opts.File = value[0]
		case "output":
			opts.OutputFn = value[0]
		}
	}
	return opts

	// The 'help' command is a special case for cobra, so doesn't need to be handled here
	// cmd.Flags().BoolVarP(&opts.Help, "help", "h", false, "display this help screen")
}
