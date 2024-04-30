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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	chunks "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type chunksOptionsInternal struct {
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
func (opts *chunksOptionsInternal) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// ChunksBytes implements the chifra chunks command for the SDK.
func (opts *chunksOptionsInternal) ChunksBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		return fmt.Errorf("error converting chunks struct to URL values: %v", err)
	}

	return chunks.Chunks(w, values)
}

// chunksParseFunc handles special cases such as structs and enums (if any).
func chunksParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	opts, ok := target.(*chunksOptionsInternal)
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
func GetChunksOptions(args []string) (*chunksOptionsInternal, error) {
	var opts chunksOptionsInternal
	if err := assignValuesFromArgs(args, chunksParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type chunksGeneric interface {
	types.ChunkManifest |
		types.ChunkIndex |
		types.ChunkBloom |
		types.ChunkPin |
		types.ChunkAddress |
		types.ChunkAppearance |
		types.ChunkStats |
		types.Message
}

func queryChunks[T chunksGeneric](opts *chunksOptionsInternal) ([]T, *types.MetaData, error) {
	// EXISTING_CODE
	// EXISTING_CODE

	buffer := bytes.Buffer{}
	if err := opts.ChunksBytes(&buffer); err != nil {
		return nil, nil, err
	}

	var result Result[T]
	if err := json.Unmarshal(buffer.Bytes(), &result); err != nil {
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
}

// toInternal converts the SDK options to the internal options format.
func (opts *ChunksOptions) toInternal() *chunksOptionsInternal {
	return &chunksOptionsInternal{
		BlockIds:   opts.BlockIds,
		Check:      opts.Check,
		Pin:        opts.Pin,
		Publish:    opts.Publish,
		Publisher:  opts.Publisher,
		Remote:     opts.Remote,
		Belongs:    opts.Belongs,
		FirstBlock: opts.FirstBlock,
		LastBlock:  opts.LastBlock,
		MaxAddrs:   opts.MaxAddrs,
		Deep:       opts.Deep,
		Rewrite:    opts.Rewrite,
		List:       opts.List,
		Unpin:      opts.Unpin,
		Count:      opts.Count,
		Sleep:      opts.Sleep,
		Globals:    opts.Globals,
	}
}

// EXISTING_CODE
// EXISTING_CODE
