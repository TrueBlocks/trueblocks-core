// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ExportOptions) ValidateExport() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if len(opts.Globals.File) == 0 {
		err := validate.ValidateAtLeastOneAddr(opts.Addrs)
		if err != nil {
			return err
		}
	}

	if opts.Unripe && opts.Staging {
		return validate.Usage("Please choose only one of {0} or {1}", "--staging", "--unripe")
	}

	if opts.Globals.TestMode && (opts.Staging || opts.Unripe) {
		return validate.Usage("--staging and --unripe are disabled for testing.")
	}

	if opts.Count && (opts.Logs || opts.Receipts || opts.Traces || opts.Statements || opts.Neighbors) {
		return validate.Usage("The {0} option is only available with transactional options.", "--count")
	}

	if !opts.Logs && len(opts.Emitter) > 0 {
		return validate.Usage("The {0} option is only available with the {1} option.", "--emitter", "--logs")
	}

	if !opts.Logs && len(opts.Topics) > 0 {
		return validate.Usage("The {0} option is only available with the {1} option.", "--topic", "--logs")
	}

	if !opts.Statements && len(opts.Asset) > 0 {
		return validate.Usage("The {0} option is only available with the {1} option.", "--asset", "--statements")
	}

	if !opts.Traces && opts.Factory {
		return validate.Usage("The {0} option is only available with the {1} option.", "--factory", "--traces")
	}

	if len(opts.Fourbytes) > 0 && (opts.Logs || opts.Receipts || opts.Statements || opts.Appearances) {
		return validate.Usage("The {0} option is only available with the {1} option.", "--fourbyte", "no option or the --accounting")
	}

	if opts.Accounting && (opts.Appearances || opts.Logs || opts.Receipts || opts.Traces || opts.Statements || opts.Neighbors) {
		return validate.Usage("The {0} option is not available with other options.", "--accounting")
	}

	if opts.Accounting && len(opts.Addrs) != 1 {
		return validate.Usage("The {0} option is allows with only a single address.", "--accounting")
	}

	if opts.Accounting && opts.Globals.Chain != "mainnet" {
		logger.Log(logger.Warning, "The --accounting option reports a spotPrice of one for all assets on non-mainnet chains.")
	}

	if !opts.Count && len(opts.Addrs) == 0 {
		return validate.Usage("You must provide at least one Ethereum address for this command.")
	}

	bloomZero := cache.NewCachePath(opts.Globals.Chain, cache.Index_Bloom)
	path := bloomZero.GetFullPath("000000000-000000000")
	if !file.FileExists(path) {
		msg := "The bloom filter for block zero (000000000-000000000.bloom) was not found. You must run "
		msg += "'chifra init' (and allow it to complete) or 'chifra scrape' before using this command."
		return validate.Usage(msg)
	}

	return opts.Globals.ValidateGlobals()
}
