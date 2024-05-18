// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package chunksPkg

import (
	// EXISTING_CODE
	"encoding/json"
	"io"
	"net/http"
	"net/url"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	// EXISTING_CODE
)

// ChunksOptions provides all command options for the chifra chunks command.
type ChunksOptions struct {
	Mode       string                   `json:"mode,omitempty"`       // The type of data to process
	Blocks     []string                 `json:"blocks,omitempty"`     // An optional list of blocks to intersect with chunk ranges
	BlockIds   []identifiers.Identifier `json:"blockIds,omitempty"`   // Block identifiers
	Check      bool                     `json:"check,omitempty"`      // Check the manifest, index, or blooms for internal consistency
	Pin        bool                     `json:"pin,omitempty"`        // Pin the manifest or each index chunk and bloom
	Publish    bool                     `json:"publish,omitempty"`    // Publish the manifest to the Unchained Index smart contract
	Publisher  string                   `json:"publisher,omitempty"`  // For some query options, the publisher of the index
	Truncate   base.Blknum              `json:"truncate,omitempty"`   // Truncate the entire index at this block (requires a block identifier)
	Remote     bool                     `json:"remote,omitempty"`     // Prior to processing, retrieve the manifest from the Unchained Index smart contract
	Belongs    []string                 `json:"belongs,omitempty"`    // In index mode only, checks the address(es) for inclusion in the given index chunk
	Diff       bool                     `json:"diff,omitempty"`       // Compare two index portions (see notes)
	FirstBlock base.Blknum              `json:"firstBlock,omitempty"` // First block to process (inclusive)
	LastBlock  base.Blknum              `json:"lastBlock,omitempty"`  // Last block to process (inclusive)
	MaxAddrs   uint64                   `json:"maxAddrs,omitempty"`   // The max number of addresses to process in a given chunk
	Deep       bool                     `json:"deep,omitempty"`       // If true, dig more deeply during checking (manifest only)
	Rewrite    bool                     `json:"rewrite,omitempty"`    // For the --pin --deep mode only, writes the manifest back to the index folder (see notes)
	List       bool                     `json:"list,omitempty"`       // For the pins mode only, list the remote pins
	Unpin      bool                     `json:"unpin,omitempty"`      // For the pins mode only, if true reads local ./unpins file for valid CIDs and remotely unpins each (skips non-CIDs)
	Count      bool                     `json:"count,omitempty"`      // For the pins mode only, display only the count of records
	Tag        string                   `json:"tag,omitempty"`        // Visits each chunk and updates the headers with the supplied version string (vX.Y.Z-str)
	Sleep      float64                  `json:"sleep,omitempty"`      // For --remote pinning only, seconds to sleep between API calls
	Globals    globals.GlobalOptions    `json:"globals,omitempty"`    // The global options
	Conn       *rpc.Connection          `json:"conn,omitempty"`       // The connection to the RPC server
	BadFlag    error                    `json:"badFlag,omitempty"`    // An error flag if needed
	// EXISTING_CODE
	PublisherAddr base.Address `json:"-"`
	// EXISTING_CODE
}

var defaultChunksOptions = ChunksOptions{
	Truncate:  base.NOPOSN,
	LastBlock: base.NOPOSN,
	MaxAddrs:  base.NOPOS,
}

// testLog is used only during testing to export the options for this test case.
func (opts *ChunksOptions) testLog() {
	logger.TestLog(len(opts.Mode) > 0, "Mode: ", opts.Mode)
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(opts.Check, "Check: ", opts.Check)
	logger.TestLog(opts.Pin, "Pin: ", opts.Pin)
	logger.TestLog(opts.Publish, "Publish: ", opts.Publish)
	logger.TestLog(len(opts.Publisher) > 0, "Publisher: ", opts.Publisher)
	logger.TestLog(opts.Truncate != base.NOPOSN, "Truncate: ", opts.Truncate)
	logger.TestLog(opts.Remote, "Remote: ", opts.Remote)
	logger.TestLog(len(opts.Belongs) > 0, "Belongs: ", opts.Belongs)
	logger.TestLog(opts.Diff, "Diff: ", opts.Diff)
	logger.TestLog(opts.FirstBlock != 0, "FirstBlock: ", opts.FirstBlock)
	logger.TestLog(opts.LastBlock != base.NOPOSN && opts.LastBlock != 0, "LastBlock: ", opts.LastBlock)
	logger.TestLog(opts.MaxAddrs != base.NOPOS, "MaxAddrs: ", opts.MaxAddrs)
	logger.TestLog(opts.Deep, "Deep: ", opts.Deep)
	logger.TestLog(opts.Rewrite, "Rewrite: ", opts.Rewrite)
	logger.TestLog(opts.List, "List: ", opts.List)
	logger.TestLog(opts.Unpin, "Unpin: ", opts.Unpin)
	logger.TestLog(opts.Count, "Count: ", opts.Count)
	logger.TestLog(len(opts.Tag) > 0, "Tag: ", opts.Tag)
	logger.TestLog(opts.Sleep != float64(0.0), "Sleep: ", opts.Sleep)
	opts.Conn.TestLog(opts.getCaches())
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *ChunksOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// chunksFinishParseApi finishes the parsing for server invocations. Returns a new ChunksOptions.
func chunksFinishParseApi(w http.ResponseWriter, r *http.Request) *ChunksOptions {
	values := r.URL.Query()
	if r.Header.Get("User-Agent") == "testRunner" {
		values.Set("testRunner", "true")
	}
	return ChunksFinishParseInternal(w, values)
}

func ChunksFinishParseInternal(w io.Writer, values url.Values) *ChunksOptions {
	copy := defaultChunksOptions
	copy.Globals.Caps = getCaps()
	opts := &copy
	opts.Truncate = base.NOPOSN
	opts.LastBlock = base.NOPOSN
	opts.MaxAddrs = base.NOPOS
	for key, value := range values {
		switch key {
		case "mode":
			opts.Mode = value[0]
		case "blocks":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Blocks = append(opts.Blocks, s...)
			}
		case "check":
			opts.Check = true
		case "pin":
			opts.Pin = true
		case "publish":
			opts.Publish = true
		case "publisher":
			opts.Publisher = value[0]
		case "truncate":
			opts.Truncate = base.MustParseBlknum(value[0])
		case "remote":
			opts.Remote = true
		case "belongs":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Belongs = append(opts.Belongs, s...)
			}
		case "diff":
			opts.Diff = true
		case "firstBlock":
			opts.FirstBlock = base.MustParseBlknum(value[0])
		case "lastBlock":
			opts.LastBlock = base.MustParseBlknum(value[0])
		case "maxAddrs":
			opts.MaxAddrs = base.MustParseUint64(value[0])
		case "deep":
			opts.Deep = true
		case "rewrite":
			opts.Rewrite = true
		case "list":
			opts.List = true
		case "unpin":
			opts.Unpin = true
		case "count":
			opts.Count = true
		case "tag":
			opts.Tag = value[0]
		case "sleep":
			opts.Sleep = base.MustParseFloat64(value[0])
		default:
			if !copy.Globals.Caps.HasKey(key) {
				err := validate.Usage("Invalid key ({0}) in {1} route.", key, "chunks")
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
	// EXISTING_CODE
	opts.Belongs, _ = opts.Conn.GetEnsAddresses(opts.Belongs)

	return opts
}

// chunksFinishParse finishes the parsing for command line invocations. Returns a new ChunksOptions.
func chunksFinishParse(args []string) *ChunksOptions {
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
	if len(args) > 0 {
		opts.Mode = args[0]
		for i, arg := range args {
			if i > 0 {
				if base.IsValidAddress(arg) {
					opts.Belongs = append(opts.Belongs, arg)
				} else {
					opts.Blocks = append(opts.Blocks, arg)
				}
			}
		}
	}
	if opts.Truncate == 0 {
		opts.Truncate = base.NOPOSN
	}
	if opts.LastBlock == 0 {
		opts.LastBlock = base.NOPOSN
	}
	if opts.MaxAddrs == 0 {
		opts.MaxAddrs = base.NOPOS
	}
	getDef := func(def string) string {
		if opts.Truncate != base.NOPOSN || len(opts.Belongs) > 0 || opts.Pin {
			return "json"
		}
		return def
	}
	defFmt = getDef(defFmt)
	// EXISTING_CODE
	opts.Belongs, _ = opts.Conn.GetEnsAddresses(opts.Belongs)
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}

	return opts
}

func GetOptions() *ChunksOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultChunksOptions
}

func getCaps() caps.Capability {
	var capabilities caps.Capability // capabilities for chifra chunks
	capabilities = capabilities.Add(caps.Default)
	// EXISTING_CODE
	// EXISTING_CODE
	return capabilities
}

func ResetOptions(testMode bool) {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultChunksOptions = ChunksOptions{}
	globals.SetDefaults(&defaultChunksOptions.Globals)
	defaultChunksOptions.Globals.TestMode = testMode
	defaultChunksOptions.Globals.Writer = w
	// EXISTING_CODE
	// EXISTING_CODE
	defaultChunksOptions.Globals.Caps = getCaps()
}

func (opts *ChunksOptions) getCaches() (m map[string]bool) {
	// EXISTING_CODE
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
