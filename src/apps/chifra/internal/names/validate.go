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

	if Options.Tags && anyBase() {
		return validate.Usage("The {0} option is not available{1}.", "--tags", " with any other option")
	}

	if Options.Collections && anyBase() {
		return validate.Usage("The {0} option is not available{1}.", "--collection", " with any other option")
	}

	return opts.Globals.ValidateGlobals()
}

func anyBase() bool {
	return Options.Expand ||
		Options.MatchCase ||
		Options.All ||
		Options.Custom ||
		Options.Prefund ||
		Options.Named ||
		Options.Addr ||
		Options.ToCustom ||
		Options.Clean
}
