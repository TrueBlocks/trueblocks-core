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
	abis "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type AbisOptions struct {
	// EXISTING_CODE
	Addrs     []base.Address
	Known     bool
	Proxy_For base.Address
	Find      []string
	Hint      []string
	Encode    string

	// EXISTING_CODE
}

// Abis implements the chifra abis command for the SDK.
func (opts *AbisOptions) Abis(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	// EXISTING_CODE

	return abis.Abis(w, values)
}

// EXISTING_CODE
// EXISTING_CODE

