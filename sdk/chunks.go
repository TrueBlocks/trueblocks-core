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
	"encoding/json"
	"fmt"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	// EXISTING_CODE
)

type ChunksOptions struct {
	BlockIds   []string          `json:"blocks,omitempty"`
	Check      bool              `json:"check,omitempty"`
	Pin        bool              `json:"pin,omitempty"`
	Publish    bool              `json:"publish,omitempty"`
	Publisher  base.Address      `json:"publisher,omitempty"`
	Remote     bool              `json:"remote,omitempty"`
	Belongs    []string          `json:"belongs,omitempty"`
	FirstBlock base.Blknum       `json:"firstBlock,omitempty"`
	LastBlock  base.Blknum       `json:"lastBlock,omitempty"`
	MaxAddrs   uint64            `json:"maxAddrs,omitempty"`
	Deep       bool              `json:"deep,omitempty"`
	Rewrite    bool              `json:"rewrite,omitempty"`
	List       bool              `json:"list,omitempty"`
	Unpin      bool              `json:"unpin,omitempty"`
	Sleep      float64           `json:"sleep,omitempty"`
	RenderCtx  *output.RenderCtx `json:"-"`
	Globals
}

// String implements the stringer interface
func (opts ChunksOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// ChunksManifest implements the chifra chunks manifest command.
func (opts *ChunksOptions) ChunksManifest() ([]types.ChunkManifest, *types.MetaData, error) {
	in := opts.toInternal()
	in.Mode = CMManifest
	return queryChunks[types.ChunkManifest](in)
}

// ChunksIndex implements the chifra chunks index command.
func (opts *ChunksOptions) ChunksIndex() ([]types.ChunkIndex, *types.MetaData, error) {
	in := opts.toInternal()
	in.Mode = CMIndex
	return queryChunks[types.ChunkIndex](in)
}

// ChunksBlooms implements the chifra chunks blooms command.
func (opts *ChunksOptions) ChunksBlooms() ([]types.ChunkBloom, *types.MetaData, error) {
	in := opts.toInternal()
	in.Mode = CMBlooms
	return queryChunks[types.ChunkBloom](in)
}

// ChunksPins implements the chifra chunks pins command.
func (opts *ChunksOptions) ChunksPins() ([]types.ChunkPin, *types.MetaData, error) {
	in := opts.toInternal()
	in.Mode = CMPins
	return queryChunks[types.ChunkPin](in)
}

// ChunksAddresses implements the chifra chunks addresses command.
func (opts *ChunksOptions) ChunksAddresses() ([]types.ChunkAddress, *types.MetaData, error) {
	in := opts.toInternal()
	in.Mode = CMAddresses
	return queryChunks[types.ChunkAddress](in)
}

// ChunksAppearances implements the chifra chunks appearances command.
func (opts *ChunksOptions) ChunksAppearances() ([]types.ChunkAppearance, *types.MetaData, error) {
	in := opts.toInternal()
	in.Mode = CMAppearances
	return queryChunks[types.ChunkAppearance](in)
}

// ChunksStats implements the chifra chunks stats command.
func (opts *ChunksOptions) ChunksStats() ([]types.ChunkStats, *types.MetaData, error) {
	in := opts.toInternal()
	in.Mode = CMStats
	return queryChunks[types.ChunkStats](in)
}

// ChunksTruncate implements the chifra chunks --truncate command.
func (opts *ChunksOptions) ChunksTruncate(val base.Blknum) ([]types.Message, *types.MetaData, error) {
	in := opts.toInternal()
	in.Truncate = val
	return queryChunks[types.Message](in)
}

// ChunksDiff implements the chifra chunks --diff command.
func (opts *ChunksOptions) ChunksDiff() ([]types.Message, *types.MetaData, error) {
	in := opts.toInternal()
	in.Diff = true
	return queryChunks[types.Message](in)
}

// ChunksCount implements the chifra chunks --count command.
func (opts *ChunksOptions) ChunksCount() ([]types.Count, *types.MetaData, error) {
	in := opts.toInternal()
	in.Count = true
	return queryChunks[types.Count](in)
}

// ChunksTag implements the chifra chunks --tag command.
func (opts *ChunksOptions) ChunksTag(val string) ([]types.Message, *types.MetaData, error) {
	in := opts.toInternal()
	in.Tag = val
	return queryChunks[types.Message](in)
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

func SortChunkRecords(chunkrecords []types.ChunkRecord, sortSpec SortSpec) error {
	if len(sortSpec.Fields) != len(sortSpec.Order) {
		return fmt.Errorf("Fields and Order must have the same length")
	}

	sorts := make([]func(p1, p2 types.ChunkRecord) bool, len(sortSpec.Fields))
	for i, field := range sortSpec.Fields {
		if !types.IsValidChunkRecordField(field) {
			return fmt.Errorf("%s is not an ChunkRecord sort field", field)
		}
		sorts[i] = types.ChunkRecordBy(types.ChunkRecordField(field), types.SortOrder(sortSpec.Order[i]))
	}

	sort.Slice(chunkrecords, types.ChunkRecordCmp(chunkrecords, sorts...))
	return nil
}

func SortChunkStats(chunkstats []types.ChunkStats, sortSpec SortSpec) error {
	if len(sortSpec.Fields) != len(sortSpec.Order) {
		return fmt.Errorf("Fields and Order must have the same length")
	}

	sorts := make([]func(p1, p2 types.ChunkStats) bool, len(sortSpec.Fields))
	for i, field := range sortSpec.Fields {
		if !types.IsValidChunkStatsField(field) {
			return fmt.Errorf("%s is not an ChunkStats sort field", field)
		}
		sorts[i] = types.ChunkStatsBy(types.ChunkStatsField(field), types.SortOrder(sortSpec.Order[i]))
	}

	sort.Slice(chunkstats, types.ChunkStatsCmp(chunkstats, sorts...))
	return nil
}

// EXISTING_CODE
// EXISTING_CODE
