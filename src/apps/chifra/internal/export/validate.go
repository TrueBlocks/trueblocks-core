// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
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

	return opts.Globals.ValidateGlobals()
}
