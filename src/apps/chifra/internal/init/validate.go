// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package initPkg

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"

func (opts *InitOptions) validateInit() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.FirstBlock != 0 {
		return validate.Usage("{0} is not yet implemented.", "--first_block")
	}

	// Note - we don't check the index for back level since chifra init is how we upgrade the index
	// index.CheckBackLevelIndex(opts.Globals.Chain)

	if opts.Globals.TestMode {
		return validate.Usage("integration testing was skipped for chifra init")
	}

	return opts.Globals.Validate()
}
