// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package scrapePkg

import (
	// EXISTING_CODE
	"encoding/json"
	"io"
	"net/http"
	"net/url"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	// EXISTING_CODE
)

// ScrapeOptions provides all command options for the chifra scrape command.
type ScrapeOptions struct {
	BlockCnt  uint64                `json:"blockCnt,omitempty"`  // Maximum number of blocks to process per pass
	Sleep     float64               `json:"sleep,omitempty"`     // Seconds to sleep between scraper passes
	Touch     base.Blknum           `json:"touch,omitempty"`     // First block to visit when scraping (snapped back to most recent snap_to_grid mark)
	RunCount  uint64                `json:"runCount,omitempty"`  // Run the scraper this many times, then quit
	Publisher string                `json:"publisher,omitempty"` // For some query options, the publisher of the index
	DryRun    bool                  `json:"dryRun,omitempty"`    // Show the configuration that would be applied if run,no changes are made
	Settings  config.ScrapeSettings `json:"settings,omitempty"`  // Configuration items for the scrape
	Globals   globals.GlobalOptions `json:"globals,omitempty"`   // The global options
	Conn      *rpc.Connection       `json:"conn,omitempty"`      // The connection to the RPC server
	BadFlag   error                 `json:"badFlag,omitempty"`   // An error flag if needed
	// EXISTING_CODE
	PublisherAddr base.Address `json:"-"`
	// EXISTING_CODE
}

var defaultScrapeOptions = ScrapeOptions{
	BlockCnt: 2000,
	Sleep:    14,
}

// testLog is used only during testing to export the options for this test case.
func (opts *ScrapeOptions) testLog() {
	logger.TestLog(opts.BlockCnt != 2000, "BlockCnt: ", opts.BlockCnt)
	logger.TestLog(opts.Sleep != float64(14), "Sleep: ", opts.Sleep)
	logger.TestLog(opts.Touch != 0, "Touch: ", opts.Touch)
	logger.TestLog(opts.RunCount != 0, "RunCount: ", opts.RunCount)
	logger.TestLog(len(opts.Publisher) > 0, "Publisher: ", opts.Publisher)
	logger.TestLog(opts.DryRun, "DryRun: ", opts.DryRun)
	opts.Settings.TestLog(opts.Globals.Chain, opts.Globals.TestMode)
	opts.Conn.TestLog(opts.getCaches())
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *ScrapeOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// scrapeFinishParseApi finishes the parsing for server invocations. Returns a new ScrapeOptions.
func scrapeFinishParseApi(w http.ResponseWriter, r *http.Request) *ScrapeOptions {
	values := r.URL.Query()
	if r.Header.Get("User-Agent") == "testRunner" {
		values.Set("testRunner", "true")
	}
	return ScrapeFinishParseInternal(w, values)
}

func ScrapeFinishParseInternal(w io.Writer, values url.Values) *ScrapeOptions {
	copy := defaultScrapeOptions
	copy.Globals.Caps = getCaps()
	opts := &copy
	opts.BlockCnt = 2000
	opts.Sleep = 14
	opts.Settings.AppsPerChunk = 2000000
	opts.Settings.SnapToGrid = 250000
	opts.Settings.FirstSnap = 2000000
	opts.Settings.UnripeDist = 28
	opts.Settings.ChannelCount = 20
	configs := make(map[string]string, 10)
	for key, value := range values {
		switch key {
		case "blockCnt":
			opts.BlockCnt = base.MustParseUint64(value[0])
		case "sleep":
			opts.Sleep = base.MustParseFloat64(value[0])
		case "touch":
			opts.Touch = base.MustParseBlknum(value[0])
		case "runCount":
			opts.RunCount = base.MustParseUint64(value[0])
		case "publisher":
			opts.Publisher = value[0]
		case "dryRun":
			opts.DryRun = true
		case "appsPerChunk":
			configs[key] = value[0]
		case "snapToGrid":
			configs[key] = value[0]
		case "firstSnap":
			configs[key] = value[0]
		case "unripeDist":
			configs[key] = value[0]
		case "channelCount":
			configs[key] = value[0]
		case "allowMissing":
			configs[key] = value[0]
		default:
			if !copy.Globals.Caps.HasKey(key) {
				err := validate.Usage("Invalid key ({0}) in {1} route.", key, "scrape")
				if opts.BadFlag == nil || opts.BadFlag.Error() > err.Error() {
					opts.BadFlag = err
				}
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, values, opts.getCaches())
	opts.Publisher, _ = opts.Conn.GetEnsAddress(config.GetPublisher(opts.Publisher))
	opts.PublisherAddr = base.HexToAddress(opts.Publisher)

	// EXISTING_CODE
	config.SetScrapeArgs(opts.Globals.Chain, configs)
	// EXISTING_CODE

	return opts
}

// scrapeFinishParse finishes the parsing for command line invocations. Returns a new ScrapeOptions.
func scrapeFinishParse(args []string) *ScrapeOptions {
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
	opts.Publisher, _ = opts.Conn.GetEnsAddress(config.GetPublisher(opts.Publisher))
	opts.PublisherAddr = base.HexToAddress(opts.Publisher)

	// EXISTING_CODE
	configs := getConfigCmdsFromArgs()
	config.SetScrapeArgs(opts.Globals.Chain, configs)
	if len(args) == 1 && (args[0] == "run" || args[0] == "indexer") {
		// these options have been deprecated, so do nothing
	} else if len(args) > 1 {
		opts.BadFlag = validate.Usage("Invalid argument {0}", args[0])
	}
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}

	return opts
}

func GetOptions() *ScrapeOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultScrapeOptions
}

func getCaps() caps.Capability {
	var capabilities caps.Capability // capabilities for chifra scrape
	capabilities = capabilities.Add(caps.Verbose)
	capabilities = capabilities.Add(caps.Version)
	capabilities = capabilities.Add(caps.Noop)
	capabilities = capabilities.Add(caps.NoColor)
	capabilities = capabilities.Add(caps.Chain)
	// EXISTING_CODE
	// EXISTING_CODE
	return capabilities
}

func ResetOptions(testMode bool) {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultScrapeOptions = ScrapeOptions{}
	globals.SetDefaults(&defaultScrapeOptions.Globals)
	defaultScrapeOptions.Globals.TestMode = testMode
	defaultScrapeOptions.Globals.Writer = w
	// EXISTING_CODE
	// EXISTING_CODE
	defaultScrapeOptions.Globals.Caps = getCaps()
}

func (opts *ScrapeOptions) getCaches() (m map[string]bool) {
	// EXISTING_CODE
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
