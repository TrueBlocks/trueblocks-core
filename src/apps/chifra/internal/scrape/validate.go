// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package scrapePkg

import (
	"fmt"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/validate"
)

// TODO: this is a much more elegant way to do error strings:
// TODO: https://github.com/storj/uplink/blob/v1.7.0/bucket.go#L19

func (opts *ScrapeOptions) validateScrape() error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if !config.IsChainConfigured(chain) {
		return validate.Usage("chain {0} is not properly configured.", chain)
	}

	if opts.Notify {
		if !NotifyConfigured() {
			return validate.Usage("The {0} feature is {1}.", "--notify", "not properly configured. See the README.md")
		}
		if !config.IpfsRunning() {
			return validate.Usage("The {0} option requires {1}.", "--notify", "a locally running IPFS daemon")
		}
	} else if !testMode && NotifyConfigured() && !opts.DryRun {
		msg := validate.Usage("The notify feature is configured but not running. Enable it with the {0} flag.", "--notify").Error()
		logger.Warn(msg)
	}

	err, ok := opts.Conn.IsNodeTracing()
	if !ok {
		return validate.Usage("{0} requires {1}, try {2} instead. Error: {3}", "chifra scrape", "tracing", "chifra init", err.Error())
	}

	if !opts.Conn.IsNodeArchive() {
		return validate.Usage("{0} requires {1}, try {2} instead.", "chifra scrape", "an archive node", "chifra init")
	}

	if opts.Globals.IsApiMode() {
		return validate.Usage("chifra scrape is not available in API mode")
	}

	if opts.Sleep < .25 {
		return validate.Usage("The {0} option ({1}) must {2}.", "--sleep", fmt.Sprintf("%f", opts.Sleep), "be at least .25")
	}

	if opts.BlockCnt < 10 {
		return validate.Usage("Specify at least {0} with {0}.", "10 blocks per round", "chifra scrape")
	}

	// We can't really test this code, so we just report and quit
	if opts.Globals.TestMode && !opts.DryRun {
		return validate.Usage("Cannot test block scraper")
	}

	meta, err := opts.Conn.GetMetaData(opts.Globals.TestMode)
	if err != nil {
		return err
	}
	m := max(meta.Ripe, max(meta.Staging, meta.Finalized)) + 1
	if !opts.DryRun && m > meta.Latest {
		fmt.Println(validate.Usage("The index ({0}) is ahead of the chain ({1}).", fmt.Sprintf("%d", m), fmt.Sprintf("%d", meta.Latest)))
	}

	if opts.Touch > 0 && opts.Touch < meta.Finalized {
		return validate.Usage("The index is already finalized ({0}) past the touch block ({1}).", fmt.Sprintf("%d", meta.Finalized), fmt.Sprintf("%d", opts.Touch))
	}

	if len(opts.Publisher) > 0 {
		err := validate.ValidateExactlyOneAddr([]string{opts.Publisher})
		if err != nil {
			return err
		}
	}

	ret := opts.Globals.Validate()

	pidPath := opts.getPidFilePath()
	if file.FileExists(pidPath) {
		pid := base.MustParseInt64(file.AsciiFileToString(pidPath))
		// fmt.Println("Pid file exists with contents:", pid)
		if running, err := utils.PidExists(pid); err == nil && running {
			return validate.Usage("The {0} is already running. If it is not, remove {1} and try again.", "scraper", pidPath)
		} else if err != nil {
			return validate.Usage("The {0} is already running. If it is not, remove {1} and try again.", "scraper", pidPath)
		}
		// fmt.Println("Removing pid file")
		os.Remove(pidPath)
	}
	// If we've gotten this far, we're the only one running. As we enter the forever
	// loop, we want to make sure no-one else runs. We do this by writing our pid to
	// a file. Note that this probably doesn't work in the server.
	if err := os.MkdirAll(filepath.Dir(pidPath), 0700); err != nil {
		return fmt.Errorf("creating temp directory: %w", err)
	}
	if err := file.StringToAsciiFile(pidPath, fmt.Sprintf("%d", os.Getpid())); err != nil {
		return fmt.Errorf("creating scrape.pid file: %w", err)
	}

	return ret
}
