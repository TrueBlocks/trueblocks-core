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

type NamesOptions struct {
	Terms     []string     `json:"terms,omitempty"`
	Expand    bool         `json:"expand,omitempty"`
	MatchCase bool         `json:"matchCase,omitempty"`
	All       bool         `json:"all,omitempty"`
	Custom    bool         `json:"custom,omitempty"`
	Prefund   bool         `json:"prefund,omitempty"`
	Clean     bool         `json:"clean,omitempty"`
	Regular   bool         `json:"regular,omitempty"`
	DryRun    bool         `json:"dryRun,omitempty"`
	Autoname  base.Address `json:"autoname,omitempty"`
	Create    bool         `json:"create,omitempty"`
	Update    bool         `json:"update,omitempty"`
	Delete    bool         `json:"delete,omitempty"`
	Undelete  bool         `json:"undelete,omitempty"`
	Remove    bool         `json:"remove,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *NamesOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Names implements the chifra names command.
func (opts *NamesOptions) Names() ([]types.Name, *types.MetaData, error) {
	in := opts.toInternal()
	return queryNames[types.Name](in)
}

// NamesAddr implements the chifra names --addr command.
func (opts *NamesOptions) NamesAddr() ([]base.Address, *types.MetaData, error) {
	in := opts.toInternal()
	in.Addr = true
	return queryNames[base.Address](in)
}

// NamesTags implements the chifra names --tags command.
func (opts *NamesOptions) NamesTags() ([]string, *types.MetaData, error) {
	in := opts.toInternal()
	in.Tags = true
	return queryNames[string](in)
}

// No enums
// EXISTING_CODE
// EXISTING_CODE
