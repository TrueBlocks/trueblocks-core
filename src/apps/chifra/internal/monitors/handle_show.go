// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
)

func (opts *MonitorsOptions) HandleShow(rCtx *output.RenderCtx) error {
	_ = rCtx
	logger.ShouldNotHappen("chifra monitors")
	return nil
}
