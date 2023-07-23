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

	if opts.Globals.TestMode {
		return validate.Usage("integration testing was skipped for chifra init")
	}

	return opts.Globals.Validate()
}
