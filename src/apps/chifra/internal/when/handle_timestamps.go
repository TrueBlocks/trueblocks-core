// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
)

// HandleTimestamps handles chifra when --timestamps
func (opts *WhenOptions) HandleTimestamps(rCtx *output.RenderCtx) error {
	var err error
	if opts.Update {
		err = opts.HandleTimestampsUpdate(rCtx)
	} else if opts.Count {
		err = opts.HandleTimestampsCount(rCtx)
	} else if opts.Truncate != base.NOPOSN {
		err = opts.HandleTimestampsTruncate(rCtx)
	} else if opts.Check {
		err = opts.HandleTimestampsCheck(rCtx)
	} else if opts.Repair {
		err = opts.HandleTimestampsRepair(rCtx)
	} else {
		err = opts.HandleTimestampsShow(rCtx)
	}
	return err
}
