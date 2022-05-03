// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	tslibPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

func (opts *WhenOptions) HandleWhenList() error {

	result, err := tslibPkg.GetSpecials(opts.Globals.Chain)
	if err != nil {
		return err
	}

	// TODO: Fix export without arrays
	if opts.Globals.ApiMode {
		opts.Globals.Respond2(opts.Globals.Writer, result, false)

	} else {
		err := opts.Globals.Output2(os.Stdout, opts.Globals.Format, result, false)
		if err != nil {
			logger.Log(logger.Error, err)
		}
	}

	return nil
}
