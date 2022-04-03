// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ChunksOptions) ValidateChunks() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if len(opts.Extract) == 0 {
		return validate.Usage("The --extract option may not be empty.")
	}

	err := validate.ValidateEnum("--extract", opts.Extract, "[stats|pins|blooms|index|header|addresses|appearances]")
	if err != nil {
		return err
	}

	return opts.Globals.ValidateGlobals()
}
