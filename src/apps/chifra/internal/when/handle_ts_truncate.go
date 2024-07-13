// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

// HandleTimestampsTruncate handles chifra when --timestamps --drop <bn>
func (opts *WhenOptions) HandleTimestampsTruncate() error {
	_ = context.TODO()
	chain := opts.Globals.Chain

	if err := tslib.Truncate(chain, opts.Truncate+1); err != nil {
		return err
	}

	logger.Info("The timestamps file was trucated to block", opts.Truncate)
	return nil
}
