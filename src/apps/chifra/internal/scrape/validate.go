// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package scrapePkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/migrate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/pinCfg"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrapeCfg"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// TODO: this is a much more elegant way to do error strings:
// TODO: https://github.com/storj/uplink/blob/v1.7.0/bucket.go#L19

func (opts *ScrapeOptions) validateScrape() error {
	// First, we need to pick up the settings TODO: Should be auto-generated code somehow
	opts.Settings, _ = scrapeCfg.GetSettings(opts.Globals.Chain, &opts.Settings)

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.Sleep < .25 {
		return validate.Usage("The {0} option ({1}) must {2}.", "--sleep", fmt.Sprintf("%f", opts.Sleep), "be at least .25")
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
		fmt.Println(validate.Usage("The index ({0}) is ahead of the chain ({1}).", fmt.Sprintf("%d", m), fmt.Sprintf("%d", meta.Latest)))
	}

	if opts.Pin {
		pinataKey, pinataSecret, estuaryKey := pinCfg.PinningKeys(opts.Globals.Chain)
		if (pinataKey == "" || pinataSecret == "") && estuaryKey == "" {
			return validate.Usage("The {0} option requires {1}", "--pin", "your pinning service's api key")

		} else if !pinning.LocalDaemonRunning() {
			return validate.Usage("The {0} option requires {1}", "--pin", "a locally running IPFS daemon or --remote")

		}
	}

	// Note this does not return if a migration is needed
	migrate.CheckBackLevelIndex(opts.Globals.Chain, true)

	return opts.Globals.Validate()
}
