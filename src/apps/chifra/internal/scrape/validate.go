// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package scrapePkg

import (
	"fmt"

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
		return validate.Usage("Please choose at least one of {0}.", "[indexer|monitors|both]")

	} else {
		for _, arg := range opts.Modes {
			err := validate.ValidateEnum("mode", arg, "[indexer|monitors|both]")
			if err != nil {
				return err
			}
		}
	}

	if opts.Action == "" {
		opts.Action = "run"
	}
	err := validate.ValidateEnum("action", opts.Action, "[toggle|run|restart|pause|quit]")
	if err != nil {
		return err
	}

	if opts.Sleep < .5 {
		return validate.Usage("The {0} option ({1}) must {2}.", "--sleep", fmt.Sprintf("%f", opts.Sleep), "be greater than .5")
	}

	if opts.Pin && !hasIndexerFlag(opts.Modes[0]) {
		return validate.Usage("The {0} option is available only with {1}.", "--pin", "the indexer")
	}

	if opts.Publish && !hasIndexerFlag(opts.Modes[0]) {
		return validate.Usage("The {0} option is available only with {1}.", "--publish", "the indexer")
	}

	return opts.Globals.ValidateGlobals()
}
