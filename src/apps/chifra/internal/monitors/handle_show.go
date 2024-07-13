// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func (opts *MonitorsOptions) HandleShow() error {
	_ = context.TODO()
	logger.Fatal("Should not happen for chifra monitors.")
	return nil
}
