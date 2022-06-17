// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package scrapePkg

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// TODO: this is a much more elegant way to do error strings:
// TODO: https://github.com/storj/uplink/blob/v1.7.0/bucket.go#L19

func (opts *ScrapeOptions) ValidateScrape() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if len(opts.Modes) == 0 {
		return validate.Usage("Please choose at least one of {0}.", "[run|stop]")

	} else {
		for _, arg := range opts.Modes {
			// TODO: BOGUS - this should be noted as deprecated somehow
			arg = strings.Replace(arg, "indexer", "run", -1)
			err := validate.ValidateEnum("mode", arg, "[run|stop]")
			if err != nil {
				return err
			}
		}
	}

	// if opts.Blaze {
	// 	// TODO: StartBlock and RipeBlock must be sent with the --blaze option
	// } else {
	// 	// TODO: StartBlock and RipeBlock can only be sent with the --blaze option
	// }

	if opts.Sleep < .25 {
		return validate.Usage("The {0} option ({1}) must {2}.", "--sleep", fmt.Sprintf("%f", opts.Sleep), "be at least .25")
	}

	return opts.Globals.ValidateGlobals()
}
