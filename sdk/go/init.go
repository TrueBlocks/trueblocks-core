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
	initPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type InitOptions struct {
	// EXISTING_CODE
	All         bool
	Dry_Run     bool
	Publisher   base.Address
	First_Block base.Blknum
	Sleep       float64
	Globals

	// EXISTING_CODE
}

// Init implements the chifra init command for the SDK.
func (opts *InitOptions) Init(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	// EXISTING_CODE

	return initPkg.Init(w, values)
}

// EXISTING_CODE
// EXISTING_CODE

