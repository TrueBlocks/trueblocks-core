// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
package main

// EXISTING_CODE
import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	sdk "github.com/TrueBlocks/trueblocks-sdk/v5"
)

// EXISTING_CODE

// DoChunks tests the Chunks sdk function
func DoChunks() {
	file.EstablishFolder("sdkFuzzer-output/chunks")
	opts := sdk.ChunksOptions{}
	ShowHeader("DoChunks", opts)

	globs := noCache(noEther(globals))
	// check := []bool{false, true}
	pin := []bool{false, true}
	publish := []bool{false, true}
	remote := []bool{false, true}
	belongs := fuzzBelongs
	deep := []bool{false, true}
	rewrite := []bool{false, true}
	unpin := []bool{false, true}
	dryRun := []bool{false, true}
	metadata := []bool{false, true}
	// firstBlock is a <blknum> --other
	// lastBlock is a <blknum> --other
	// maxAddrs is a <uint64> --other
	// blocks is not fuzzed
	// publisher is not fuzzed
	// truncate is not fuzzed
	// sleep is not fuzzed
	// Fuzz Loop
	// EXISTING_CODE
	_ = dryRun
	_ = belongs
	_ = globs
	_ = pin
	_ = publish
	_ = remote
	_ = deep
	_ = rewrite
	_ = unpin
	_ = metadata
	opts = sdk.ChunksOptions{
		FirstBlock: 0,
		LastBlock:  base.NOPOSN,
		MaxAddrs:   base.NOPOS,
	}

	// BlockIds   []string     `json:"blocks,omitempty"`
	// Check      bool         `json:"check,omitempty"`
	// Pin        bool         `json:"pin,omitempty"`
	// Publish    bool         `json:"publish,omitempty"`
	// Publisher  base.Address `json:"publisher,omitempty"`
	// Remote     bool         `json:"remote,omitempty"`
	// Belongs    []string     `json:"belongs,omitempty"`
	// Deep       bool         `json:"deep,omitempty"`
	// Rewrite    bool         `json:"rewrite,omitempty"`
	// List       bool         `json:"list,omitempty"`
	// Unpin      bool         `json:"unpin,omitempty"`
	// Count      bool         `json:"count,omitempty"`
	// Sleep      float64      `json:"sleep,omitempty"`
	// chunks,command,default|
	baseFn := "chunks/chunks"
	fn := getFilename(baseFn, &opts.Globals)
	TestChunks("manifest", "", fn, &opts)
	TestChunks("index", "", fn, &opts)
	// TestChunks("blooms", fn, &opts)
	// TestChunks("pins", fn, &opts)
	// TestChunks("addresses", fn, &opts)
	// TestChunks("appearances", fn, &opts)
	TestChunks("stats", "", fn, &opts)

	// func (opts *ChunksOptions) ChunksTruncate(val base.Blknum) ([]types.Message, *types.MetaData, error) {
	// func (opts *ChunksOptions) ChunksDiff() ([]types.Message, *types.MetaData, error) {
	// func (opts *ChunksOptions) ChunksTag(val string) ([]types.Message, *types.MetaData, error) {
	// EXISTING_CODE
	Wait()
}

func TestChunks(which, value, fn string, opts *sdk.ChunksOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	// EXISTING_CODE
	// EXISTING_CODE

	switch which {
	case "manifest":
		if manifest, _, err := opts.ChunksManifest(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, manifest); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "index":
		if index, _, err := opts.ChunksIndex(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, index); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "blooms":
		if blooms, _, err := opts.ChunksBlooms(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, blooms); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "pins":
		if pins, _, err := opts.ChunksPins(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, pins); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "addresses":
		if addresses, _, err := opts.ChunksAddresses(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, addresses); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "appearances":
		if appearances, _, err := opts.ChunksAppearances(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, appearances); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "stats":
		if stats, _, err := opts.ChunksStats(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, stats); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "truncate":
		if truncate, _, err := opts.ChunksTruncate(base.MustParseBlknum(value)); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, truncate); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "diff":
		if diff, _, err := opts.ChunksDiff(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, diff); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "list":
		if list, _, err := opts.ChunksList(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, list); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "count":
		if count, _, err := opts.ChunksCount(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, count); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "tag":
		if tag, _, err := opts.ChunksTag(value); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, tag); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	default:
		ReportError(fn, opts, fmt.Errorf("unknown which: %s", which))
		logger.Fatal("Quitting...")
		return
	}
}

// EXISTING_CODE
// EXISTING_CODE
