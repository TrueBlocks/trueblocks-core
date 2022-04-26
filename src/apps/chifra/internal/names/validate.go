// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package namesPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *NamesOptions) ValidateNames() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.Tags && opts.anyBase() {
		return validate.Usage("The {0} option is not available{1}.", "--tags", " with any other option")
	}

	if opts.Collections && opts.anyBase() {
		return validate.Usage("The {0} option is not available{1}.", "--collection", " with any other option")
	}

	return opts.Globals.ValidateGlobals()
}

func (opts *NamesOptions) anyBase() bool {
	return opts.Expand ||
		opts.MatchCase ||
		opts.All ||
		opts.Prefund ||
		opts.Named ||
		opts.Addr ||
		opts.ToCustom ||
		opts.Clean
}
