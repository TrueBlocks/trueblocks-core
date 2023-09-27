// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package scrapePkg

import (
	"fmt"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrapeCfg"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// TODO: this is a much more elegant way to do error strings:
// TODO: https://github.com/storj/uplink/blob/v1.7.0/bucket.go#L19

func (opts *ScrapeOptions) validateScrape() error {
	chain := opts.Globals.Chain

	// First, we need to pick up the settings TODO: Should be auto-generated code somehow
	opts.Settings, _ = scrapeCfg.GetSettings(chain, "blockScrape.toml", &opts.Settings)

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if !config.IsChainConfigured(chain) {
		return validate.Usage("chain {0} is not properly configured.", chain)
	}

	if !opts.Conn.IsNodeTracing() {
		return validate.Usage("{0} requires tracing, err: {1}", "chifra scrape", rpc.ErrTraceBlockMissing)
	}

	if opts.Sleep < .25 {
		return validate.Usage("The {0} option ({1}) must {2}.", "--sleep", fmt.Sprintf("%f", opts.Sleep), "be at least .25")
	}

	// We can't really test this code, so we just report and quit
	if opts.Globals.TestMode {
		return validate.Usage("Cannot test block scraper")
	}

	meta, err := opts.Conn.GetMetaData(opts.Globals.TestMode)
	if err != nil {
		return err
	}
	m := utils.Max(meta.Ripe, utils.Max(meta.Staging, meta.Finalized)) + 1
	if m > meta.Latest {
		fmt.Println(validate.Usage("The index ({0}) is ahead of the chain ({1}).", fmt.Sprintf("%d", m), fmt.Sprintf("%d", meta.Latest)))
	}

	if opts.Pin {
		if opts.Remote {
			pinataKey, pinataSecret := config.GetPinningKeys(chain)
			if pinataKey == "" || pinataSecret == "" {
				return validate.Usage("The {0} option requires {1}.", "--pin --remote", "an api key")
			}

		} else if !pinning.LocalDaemonRunning() {
			return validate.Usage("The {0} option requires {1}.", "--pin", "a locally running IPFS daemon or --remote")

		}
	}

	// Note this does not return if a migration is needed
	index.CheckBackLevelIndex(chain)

	ret := opts.Globals.Validate()

	pidPath := filepath.Join(config.PathToCache(chain), "tmp/scrape.pid")
	if file.FileExists(pidPath) {
		pid := utils.MustParseInt(file.AsciiFileToString(pidPath))
		// fmt.Println("Pid file exists with contents:", pid)
		if running, err := utils.PidExists(pid); err == nil && running {
			return validate.Usage("The {0} is already be running. If it is not, remove {1} and try again.", "scraper", pidPath)
		} else if err != nil {
			return validate.Usage("The {0} is already be running. If it is not, remove {1} and try again.", "scraper", pidPath)
		}
		// fmt.Println("Removing pid file")
		os.Remove(pidPath)
	}
	// If we've gotten this far, we're the only one running. As we enter the forever
	// loop, we want to make sure no-one else runs. We do this by writing our pid to
	// a file. Note that this probably doesn't work in the server.
	_ = file.StringToAsciiFile(pidPath, fmt.Sprintf("%d", os.Getpid()))

	return ret
}
