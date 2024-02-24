// Copyright 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

import (
	// EXISTING_CODE
	"io"
	"net/url"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	slurp "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type SlurpOptions struct {
	// EXISTING_CODE
	Addrs       []base.Address
	Blocks      []base.Blknum
	Types       string
	Appearances bool
	Articulate  bool
	Source      string
	Count       bool
	Page        uint64
	Per_Page    uint64
	Sleep       float64
	Globals

	// EXISTING_CODE
}

// Slurp implements the chifra slurp command for the SDK.
func (opts *SlurpOptions) Slurp(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	// EXISTING_CODE

	return slurp.Slurp(w, values)
}

// EXISTING_CODE
// EXISTING_CODE

