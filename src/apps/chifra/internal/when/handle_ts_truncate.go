// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

// HandleWhenTimestampsReset handles chifra when --timestamps --drop <bn>
func (opts *WhenOptions) HandleWhenTimestampsReset() error {
	err := tslib.Reset(opts.Globals.Chain, opts.Reset)
	if err != nil {
		return err
	}
	logger.Log(logger.Info, "The timestamps file was trucated to block", opts.Reset)
	return nil
}
