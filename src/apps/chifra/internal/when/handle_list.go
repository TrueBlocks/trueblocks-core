// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

func (opts *WhenOptions) HandleList() error {
	results, err := tslib.GetSpecials(opts.Globals.Chain)
	if err != nil {
		return err
	}
	// TODO: Fix export without arrays
	return globals.RenderSlice(&opts.Globals, results)
}
