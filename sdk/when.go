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

type WhenOptions struct {
	BlockIds []string    `json:"blocks,omitempty"`
	Truncate base.Blknum `json:"truncate,omitempty"`
	Repair   bool        `json:"repair,omitempty"`
	Check    bool        `json:"check,omitempty"`
	Update   bool        `json:"update,omitempty"`
	Deep     bool        `json:"deep,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *WhenOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// When implements the chifra when command.
func (opts *WhenOptions) When() ([]types.NamedBlock, *types.MetaData, error) {
	in := opts.toInternal()
	return queryWhen[types.NamedBlock](in)
}

// WhenList implements the chifra when --list command.
func (opts *WhenOptions) WhenList() ([]types.NamedBlock, *types.MetaData, error) {
	in := opts.toInternal()
	in.List = true
	return queryWhen[types.NamedBlock](in)
}

// WhenTimestamps implements the chifra when --timestamps command.
func (opts *WhenOptions) WhenTimestamps() ([]types.Timestamp, *types.MetaData, error) {
	in := opts.toInternal()
	in.Timestamps = true
	return queryWhen[types.Timestamp](in)
}

// WhenCount implements the chifra when --count command.
func (opts *WhenOptions) WhenCount() ([]types.TimestampCount, *types.MetaData, error) {
	in := opts.toInternal()
	in.Count = true
	return queryWhen[types.TimestampCount](in)
}

// No enums
// EXISTING_CODE
// EXISTING_CODE
