// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

import (
	// EXISTING_CODE
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"log"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	chunks "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ChunksOptions struct {
	Mode       ChunksMode   `json:"mode,omitempty"`
	BlockIds   []string     `json:"blocks,omitempty"`
	Check      bool         `json:"check,omitempty"`
	Pin        bool         `json:"pin,omitempty"`
	Publish    bool         `json:"publish,omitempty"`
	Publisher  base.Address `json:"publisher,omitempty"`
	Truncate   base.Blknum  `json:"truncate,omitempty"`
	Remote     bool         `json:"remote,omitempty"`
	Belongs    []string     `json:"belongs,omitempty"`
	Diff       bool         `json:"diff,omitempty"`
	FirstBlock base.Blknum  `json:"firstBlock,omitempty"`
	LastBlock  base.Blknum  `json:"lastBlock,omitempty"`
	MaxAddrs   base.Blknum  `json:"maxAddrs,omitempty"`
	Deep       bool         `json:"deep,omitempty"`
	Rewrite    bool         `json:"rewrite,omitempty"`
	List       bool         `json:"list,omitempty"`
	Unpin      bool         `json:"unpin,omitempty"`
	Count      bool         `json:"count,omitempty"`
	Tag        string       `json:"tag,omitempty"`
	Sleep      float64      `json:"sleep,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *ChunksOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Chunks implements the chifra chunks command for the SDK.
func (opts *ChunksOptions) Chunks(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		log.Fatalf("Error converting chunks struct to URL values: %v", err)
	}

	return chunks.Chunks(w, values)
}

// chunksParseFunc handles specail cases such as structs and enums (if any).
func chunksParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	opts, ok := target.(*ChunksOptions)
	if !ok {
		return false, fmt.Errorf("parseFunc(chunks): target is not of correct type")
	}

	if key == "mode" {
		var err error
		values := strings.Split(value, ",")
		if opts.Mode, err = enumFromChunksMode(values); err != nil {
			return false, err
		} else {
			found = true
		}
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetChunksOptions returns a filled-in options instance given a string array of arguments.
func GetChunksOptions(args []string) (*ChunksOptions, error) {
	var opts ChunksOptions
	if err := assignValuesFromArgs(args, chunksParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type chunksResult struct {
	Data []bool       `json:"data"`
	Meta rpc.MetaData `json:"meta"`
}

func (opts *ChunksOptions) Query() ([]bool, *rpc.MetaData, error) {
	chunksBuf := bytes.Buffer{}
	if err := opts.Chunks(&chunksBuf); err != nil {
		logger.Fatal(err)
	}

	var chunks chunksResult
	if err := json.Unmarshal(chunksBuf.Bytes(), &chunks); err != nil {
		return nil, nil, err
	} else {
		return chunks.Data, &chunks.Meta, nil
	}
}

type ChunksMode int

const (
	NoCHM      ChunksMode = 0
	CMManifest            = 1 << iota
	CMIndex
	CMBlooms
	CMPins
	CMAddresses
	CMAppearances
	CMStats
)

func (v ChunksMode) String() string {
	switch v {
	case NoCHM:
		return "none"
	}

	var m = map[ChunksMode]string{
		CMManifest:    "manifest",
		CMIndex:       "index",
		CMBlooms:      "blooms",
		CMPins:        "pins",
		CMAddresses:   "addresses",
		CMAppearances: "appearances",
		CMStats:       "stats",
	}

	var ret []string
	for _, val := range []ChunksMode{CMManifest, CMIndex, CMBlooms, CMPins, CMAddresses, CMAppearances, CMStats} {
		if v&val != 0 {
			ret = append(ret, m[val])
		}
	}

	return strings.Join(ret, ",")
}

func enumFromChunksMode(values []string) (ChunksMode, error) {
	if len(values) == 0 {
		return NoCHM, fmt.Errorf("no value provided for mode option")
	}

	var result ChunksMode
	for _, val := range values {
		switch val {
		case "manifest":
			result |= CMManifest
		case "index":
			result |= CMIndex
		case "blooms":
			result |= CMBlooms
		case "pins":
			result |= CMPins
		case "addresses":
			result |= CMAddresses
		case "appearances":
			result |= CMAppearances
		case "stats":
			result |= CMStats
		default:
			return NoCHM, fmt.Errorf("unknown mode: %s", val)
		}
	}

	return result, nil
}

// EXISTING_CODE
// EXISTING_CODE
