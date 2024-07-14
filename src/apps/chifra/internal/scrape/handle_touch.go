// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package scrapePkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
)

func (opts *ScrapeOptions) HandleTouch(rCtx output.RenderCtx) error {
	// TODO: implement
	_ = context.TODO()
	logger.Error("chifra scrape --touch is not yet implemented")
	return nil
}
