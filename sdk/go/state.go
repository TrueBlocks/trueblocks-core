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
	state "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type StateOptions struct {
	// EXISTING_CODE
	Addrs      []base.Address
	Blocks     []base.Blknum
	Parts      string
	Changes    bool
	No_Zero    bool
	Call       string
	Articulate bool
	Proxy_For  base.Address
	Globals

	// EXISTING_CODE
}

// State implements the chifra state command for the SDK.
func (opts *StateOptions) State(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	// EXISTING_CODE

	return state.State(w, values)
}

// EXISTING_CODE
// EXISTING_CODE

