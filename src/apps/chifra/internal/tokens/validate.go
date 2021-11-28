// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tokensPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *TokensOptions) ValidateTokens() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	// special case for tokens which don't allow --dollars display
	if Options.Globals.Dollars {
		return validate.Usage("The {0} option is not available{1}.", "--dollars", " with this tool")
	}

	return opts.Globals.ValidateGlobals()
}
