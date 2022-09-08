// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package initPkg

func (opts *InitOptions) validateInit() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	// Note - we don't check the index for back level since this is how we upgrade the index
	// index.CheckBackLevelIndex(opts.Globals.Chain)

	return opts.Globals.Validate()
}
