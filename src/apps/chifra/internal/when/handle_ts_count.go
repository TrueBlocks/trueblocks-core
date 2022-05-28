// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	tslibPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

func (opts *WhenOptions) HandleWhenTimestampCount() error {
	type TsCount struct {
		Count uint64 `json:"count"`
	}

	count, err := tslibPkg.NTimestamps(opts.Globals.Chain)
	if err != nil {
		return err
	}
	if opts.Globals.TestMode {
		count = 5000000
	}

	obj := TsCount{Count: count}

	// TODO: Fix export without arrays
	err = opts.Globals.RenderHeader(obj, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	defer opts.Globals.RenderFooter(opts.Globals.ApiMode || opts.Globals.Format == "api")
	if err != nil {
		return err
	}
	return opts.Globals.RenderObject(obj, true)
}
