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
	chunks "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ChunksOptions struct {
	// EXISTING_CODE
	Mode        string
	Blocks      []base.Blknum
	Check       bool
	Pin         bool
	Publish     bool
	Publisher   base.Address
	Truncate    base.Blknum
	Remote      bool
	Belongs     []base.Address
	Diff        bool
	First_Block base.Blknum
	Last_Block  base.Blknum
	Max_Addrs   base.Blknum
	Deep        bool
	Rewrite     bool
	List        bool
	Unpin       bool
	Count       bool
	Tag         string
	Sleep       float64

	// EXISTING_CODE
}

// Chunks implements the chifra chunks command for the SDK.
func (opts *ChunksOptions) Chunks(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	// EXISTING_CODE

	return chunks.Chunks(w, values)
}

// EXISTING_CODE
// EXISTING_CODE

