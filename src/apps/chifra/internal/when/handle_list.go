// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	tslibPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

func (opts *WhenOptions) HandleWhenList() error {
	results, err := tslibPkg.GetSpecials(opts.Globals.Chain)
	if err != nil {
		return err
	}
	return globals.RenderSlice(&opts.Globals, results)
}
