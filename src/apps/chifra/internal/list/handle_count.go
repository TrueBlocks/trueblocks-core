// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package listPkg

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
)

func (opts *ListOptions) HandleListCount(monitorArray []monitor.Monitor) error {
	results := make([]monitor.SimpleMonitor, 0, len(monitorArray))
	for _, mon := range monitorArray {
		simp := monitor.NewSimpleMonitor(mon)
		if opts.Globals.TestMode {
			simp.LastScanned = maxTestingBlock
		}
		results = append(results, simp)
	}

	// TODO: Fix export without arrays
	if opts.Globals.ApiMode {
		opts.Globals.Respond2(opts.Globals.Writer, results, opts.Globals.NoHeader)

	} else {
		err := opts.Globals.Output2(os.Stdout, results, opts.Globals.NoHeader)
		if err != nil {
			logger.Log(logger.Error, err)
		}
	}

	return nil
}
