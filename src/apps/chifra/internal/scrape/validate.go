// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package scrapePkg

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/migrate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// TODO: this is a much more elegant way to do error strings:
// TODO: https://github.com/storj/uplink/blob/v1.7.0/bucket.go#L19

func (opts *ScrapeOptions) validateScrape() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if len(opts.Modes) == 0 {
		return validate.Usage("Please choose at least one of {0}.", "[run|stop]")

	} else {
		for _, arg := range opts.Modes {
			// TODO: BOGUS - FEATURE NOTE THIS AS DEPRECATED
			arg = strings.Replace(arg, "indexer", "run", -1)
			err := validate.ValidateEnum("mode", arg, "[run|stop]")
			if err != nil {
				return err
			}
		}
	}

	if opts.Sleep < .25 {
		return validate.Usage("The {0} option ({1}) must {2}.", "--sleep", fmt.Sprintf("%f", opts.Sleep), "be at least .25")
	}

	if opts.Pin && !pinning.LocalDaemonRunning() {
		return validate.Usage("The {0} option requires {1}", "--pin", "a locally running IPFS daemon")
	}

	// We can't really test this code, so we just report and quit
	if opts.Globals.TestMode {
		return validate.Usage("Cannot test block scraper")
	}

	meta, err := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)
	if err != nil {
		return err
	}
	m := utils.Max(meta.Ripe, utils.Max(meta.Staging, meta.Finalized)) + 1
	if m > meta.Latest {
		sb := fmt.Sprintf("%d", m)
		c := fmt.Sprintf("%d", meta.Latest)
		return validate.Usage("The index ({0}) is ahead of the chain ({1}).", sb, c)
	}

	// Note this does not return if a migration is needed
	migrate.CheckBackLevelIndex(opts.Globals.Chain)

	return opts.Globals.Validate()
}
