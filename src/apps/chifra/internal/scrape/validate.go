// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package scrapePkg

import (
	"fmt"

	exportPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/export"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// TODO: this is a much more elegant way to do error strings:
// TODO: https://github.com/storj/uplink/blob/v1.7.0/bucket.go#L19

func (opts *ScrapeOptions) ValidateScrape() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.Reset == 0 {
		opts.Reset = utils.NOPOS
	}

	if opts.Reset == utils.NOPOS {
		if len(opts.Modes) == 0 {
			return validate.Usage("Please choose at least one of {0}.", "[indexer|monitors|both]")

		} else {
			for _, arg := range opts.Modes {
				err := validate.ValidateEnum("mode", arg, "[indexer|monitors|both]")
				if err != nil {
					return err
				}
				if arg == "monitors" || arg == "both" {
					// phonied up just to make sure we have bloom for block zero
					var expOpts exportPkg.ExportOptions
					expOpts.Addrs = append(expOpts.Addrs, "0x0000000000000000000000000000000000000001")
					expOpts.Globals.Chain = opts.Globals.Chain
					err := expOpts.ValidateExport()
					if err != nil {
						return validate.Usage(err.Error())
					}
				}
			}
		}
	}

	// if opts.Blaze {
	// 	// TODO: StartBlock and RipeBlock must be sent with the --blaze option
	// } else {
	// 	// TODO: StartBlock and RipeBlock can only be sent with the --blaze option
	// }

	if opts.Action == "" {
		opts.Action = "run"
	}
	err := validate.ValidateEnum("action", opts.Action, "[toggle|run|restart|pause|quit]")
	if err != nil {
		return err
	}

	if opts.Sleep < .25 {
		return validate.Usage("The {0} option ({1}) must {2}.", "--sleep", fmt.Sprintf("%f", opts.Sleep), "be at least .25")
	}

	if opts.Pin && !hasIndexerFlag(opts.Modes[0]) {
		return validate.Usage("The {0} option is available only with {1}.", "--pin", "the indexer")
	}

	if opts.Publish && !hasIndexerFlag(opts.Modes[0]) {
		return validate.Usage("The {0} option is available only with {1}.", "--publish", "the indexer")
	}

	return opts.Globals.ValidateGlobals()
}
