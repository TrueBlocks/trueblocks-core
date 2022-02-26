// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *MonitorsOptions) ValidateMonitors() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if !opts.Globals.ApiMode && !opts.Clean {
		if len(opts.Globals.File) > 0 {
			// Do nothing
		} else {
			err := validate.ValidateAtLeastOneAddr(opts.Addrs)
			if err != nil {
				return err
			}
		}
	}

	return opts.Globals.ValidateGlobals()
}
