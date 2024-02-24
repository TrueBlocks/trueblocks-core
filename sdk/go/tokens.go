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
	tokens "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type TokensOptions struct {
	Addrs   []base.Address
	Blocks  []base.Blknum
	Parts   string
	By_Acct bool
	Changes bool
	No_Zero bool
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Tokens implements the chifra tokens command for the SDK.
func (opts *TokensOptions) Tokens(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	// EXISTING_CODE

	return tokens.Tokens(w, values)
}

// EXISTING_CODE
// EXISTING_CODE

