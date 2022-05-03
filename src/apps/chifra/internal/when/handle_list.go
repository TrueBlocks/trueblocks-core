// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	tslibPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

func (opts *WhenOptions) HandleWhenList() error {
	result, err := tslibPkg.GetSpecials(opts.Globals.Chain)
	if err != nil {
		return err
	}

	// TODO: Fix export without arrays
	return opts.Globals.OutputArray(result)
}
