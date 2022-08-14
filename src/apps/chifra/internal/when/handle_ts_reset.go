// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// HandleWhenTimestampsTruncate handles chifra when --timestamps --drop <bn>
func (opts *WhenOptions) HandleWhenTimestampsReset() error {
	// err := tslib.Truncate(opts.Globals.Chain, opts.Truncate)
	// if err != nil {
	// 	return err
	// }
	logger.Log(logger.Info, "The timestamps --reset option is not ready yet.", opts.Reset)
	return nil
}
