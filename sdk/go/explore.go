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

	explore "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ExploreOptions struct {
	Terms  []string
	Local  bool
	Google bool
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Explore implements the chifra explore command for the SDK.
func (opts *ExploreOptions) Explore(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	// EXISTING_CODE

	return explore.Explore(w, values)
}

// no enums

// EXISTING_CODE
// EXISTING_CODE

