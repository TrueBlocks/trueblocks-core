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

	traces "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type TracesOptions struct {
	TransactionIds []string
	Articulate     bool
	Filter         string
	Count          bool
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Traces implements the chifra traces command for the SDK.
func (opts *TracesOptions) Traces(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	// EXISTING_CODE

	return traces.Traces(w, values)
}

// no enums

// EXISTING_CODE
// EXISTING_CODE

