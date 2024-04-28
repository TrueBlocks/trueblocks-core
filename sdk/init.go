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

type InitOptions struct {
	Publisher  base.Address `json:"publisher,omitempty"`
	FirstBlock base.Blknum  `json:"firstBlock,omitempty"`
	Sleep      float64      `json:"sleep,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *InitOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Init implements the chifra init command.
func (opts *InitOptions) Init() ([]bool, *types.MetaData, error) {
	in := opts.toInternal()
	return queryInit[bool](in)
}

// InitAll implements the chifra init --all command.
func (opts *InitOptions) InitAll() ([]bool, *types.MetaData, error) {
	in := opts.toInternal()
	in.All = true
	return queryInit[bool](in)
}

// InitExample implements the chifra init --all command.
func (opts *InitOptions) InitExample(name string) ([]bool, *types.MetaData, error) {
	in := opts.toInternal()
	in.Example = name
	return queryInit[bool](in)
}

// InitDryRun implements the chifra init --all command.
func (opts *InitOptions) InitDryRun() ([]bool, *types.MetaData, error) {
	in := opts.toInternal()
	in.DryRun = true
	return queryInit[bool](in)
}

// No enums
// EXISTING_CODE
// EXISTING_CODE
