// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package initPkg

func (opts *InitOptions) ValidateInit() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	return opts.Globals.ValidateGlobals()
}
