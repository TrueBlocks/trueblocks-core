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
	list "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ListOptions struct {
	Addrs       []string // allow for ENS names and addresses
	Count       bool
	NoZero      bool
	Bounds      bool
	Unripe      bool
	Silent      bool
	FirstRecord uint64
	MaxRecords  uint64
	Reversed    bool
	Publisher   base.Address
	FirstBlock  base.Blknum
	LastBlock   base.Blknum
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// List implements the chifra list command for the SDK.
func (opts *ListOptions) List(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	// EXISTING_CODE

	return list.List(w, values)
}

// no enums

// EXISTING_CODE
// EXISTING_CODE

