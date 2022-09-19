// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

// HandleTimestampsRepair handles chifra when --timestamps --reset <bn> to reset a single block's timestamps (call repeatedly if needed)
func (opts *WhenOptions) HandleTimestampsRepair() error {
	if err := tslib.Freshen(opts.Globals.Chain, opts.Repair); err != nil {
		return err
	}
	if opts.Repair == 1 { // weird special case because because I don't know how to get Cobra to handle non-zero defaults
		if err := tslib.Freshen(opts.Globals.Chain, 0); err != nil {
			return err
		}
	}

	ts, _ := tslib.FromBnToTs(opts.Globals.Chain, opts.Repair)
	logger.Log(logger.Info, "The timestamp at block", opts.Repair, "was reset to", ts, "from on chain.")

	return nil
}
