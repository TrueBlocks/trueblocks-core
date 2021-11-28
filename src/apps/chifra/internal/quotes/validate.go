// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package quotesPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *QuotesOptions) ValidateQuotes() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	err := validate.ValidateEnum("--period", opts.Period, "[5|15|30|60|120|240|1440|10080|hourly|daily|weekly]")
	if err != nil {
		return err
	}

	err = validate.ValidateEnum("--types", opts.Feed, "[poloniex|maker|tellor]")
	if err != nil {
		return err
	}

	if len(opts.Globals.File) == 0 &&
		!opts.Update &&
		len(opts.Period) == 0 &&
		len(opts.Pair) == 0 &&
		len(opts.Feed) == 0 &&
		len(opts.Globals.Format) == 0 {
		return validate.Usage("Please specify at least one {0}.", "command line option")
	}

	return opts.Globals.ValidateGlobals()
}
