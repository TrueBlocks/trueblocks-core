// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// HandleTimestampsShow handles chifra when --timestamps
func (opts *WhenOptions) HandleTimestamps() error {
	_ = context.TODO()
	var err error
	if opts.Update {
		err = opts.HandleTimestampsUpdate()
	} else if opts.Count {
		err = opts.HandleTimestampsCount()
	} else if opts.Truncate != base.NOPOSN {
		err = opts.HandleTimestampsTruncate()
	} else if opts.Check {
		err = opts.HandleTimestampsCheck()
	} else if opts.Repair {
		err = opts.HandleTimestampsRepair()
	} else {
		err = opts.HandleTimestampsShow()
	}
	return err
}
