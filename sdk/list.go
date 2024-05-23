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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	// EXISTING_CODE
)

type ListOptions struct {
	Addrs       []string     `json:"addrs,omitempty"`
	NoZero      bool         `json:"noZero,omitempty"`
	Unripe      bool         `json:"unripe,omitempty"`
	Silent      bool         `json:"silent,omitempty"`
	FirstRecord uint64       `json:"firstRecord,omitempty"`
	MaxRecords  uint64       `json:"maxRecords,omitempty"`
	Reversed    bool         `json:"reversed,omitempty"`
	Publisher   base.Address `json:"publisher,omitempty"`
	FirstBlock  base.Blknum  `json:"firstBlock,omitempty"`
	LastBlock   base.Blknum  `json:"lastBlock,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts ListOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// List implements the chifra list command.
func (opts *ListOptions) List() ([]types.Appearance, *types.MetaData, error) {
	in := opts.toInternal()
	return queryList[types.Appearance](in)
}

// ListCount implements the chifra list --count command.
func (opts *ListOptions) ListCount() ([]types.Monitor, *types.MetaData, error) {
	in := opts.toInternal()
	in.Count = true
	return queryList[types.Monitor](in)
}

// ListBounds implements the chifra list --bounds command.
func (opts *ListOptions) ListBounds() ([]types.Bounds, *types.MetaData, error) {
	in := opts.toInternal()
	in.Bounds = true
	return queryList[types.Bounds](in)
}

// No enums
// EXISTING_CODE
// EXISTING_CODE
