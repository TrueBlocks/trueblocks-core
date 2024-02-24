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

	config "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ConfigOptions struct {
	// EXISTING_CODE
	Mode  string
	Paths bool
	Globals

	// EXISTING_CODE
}

// Config implements the chifra config command for the SDK.
func (opts *ConfigOptions) Config(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	// EXISTING_CODE

	return config.Config(w, values)
}

// EXISTING_CODE
// EXISTING_CODE

