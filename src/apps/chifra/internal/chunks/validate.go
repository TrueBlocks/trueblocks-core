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

	// if !Options.List && !Options.Check && len(Options.Extract) == 0 {
	// 	return validate.Usage("Please choose at least one of {0}.", "--list, --extract, or --check")
	// }

	if !Options.Check && len(Options.Extract) == 0 {
	 	return validate.Usage("Please choose at least one of {0}.", "--extract or --check")
	}

	// if (Options.List && Options.Check) || (Options.List && len(Options.Extract) > 0) {
	// 	return validate.Usage("Please choose only one of {0}.", "--list, --extract, or --check")
	// }

	// if Options.Stats && !Options.List {
	// 	return validate.Usage("The {0} option is available only with {1}.", "--stats", "--list")
	// }

	err := validate.ValidateEnum("--extract", Options.Extract, "[header|addr_table|app_table|chunks|blooms]")
	if err != nil {
		return err
	}

	return opts.Globals.ValidateGlobals()
}
