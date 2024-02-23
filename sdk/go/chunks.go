// Copyright 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

import (
	"io"

	chunks "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
)

// Chunks does chifra chunks
func Chunks(w io.Writer, options map[string]string) error {
	return chunks.Chunks(w, options)
}

// EXISTING_CODE
// EXISTING_CODE

