// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package scrapePkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ScrapeOptions) HandleTouch(rCtx *output.RenderCtx) error {
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		errorChan <- fmt.Errorf("chifra scrape --touch is not yet implemented")
	}
	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
