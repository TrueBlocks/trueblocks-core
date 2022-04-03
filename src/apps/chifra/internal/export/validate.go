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

	if opts.Accounting && opts.Globals.Chain != "mainnet" {
		logger.Log(logger.Warning, "The --accounting option reports a spotPrice of one for all assets on non-mainnet chains.")
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
