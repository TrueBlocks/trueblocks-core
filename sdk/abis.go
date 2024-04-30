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

type AbisOptions struct {
	Addrs    []string     `json:"addrs,omitempty"`
	Known    bool         `json:"known,omitempty"`
	ProxyFor base.Address `json:"proxyFor,omitempty"`
	Find     []string     `json:"find,omitempty"`
	Hint     []string     `json:"hint,omitempty"`
	Encode   string       `json:"encode,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts AbisOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Abis implements the chifra abis command.
func (opts *AbisOptions) Abis() ([]types.Function, *types.MetaData, error) {
	in := opts.toInternal()
	return queryAbis[types.Function](in)
}

// No enums
// EXISTING_CODE
// EXISTING_CODE
