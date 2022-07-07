// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

// HandleWhenDropTimestamps handles chifra when --timestamps --drop <bn>
func (opts *WhenOptions) HandleWhenDropTimestamps() error {
	err := tslib.Reset(opts.Globals.Chain, opts.Drop)
	if err != nil {
		return err
	}
	logger.Log(logger.Info, "The timestamps file was trucated to block", opts.Drop)
	return nil
}
