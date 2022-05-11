// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package listPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ListOptions) HandleListCount(monitorArray []monitor.Monitor) error {
	results := make([]types.SimpleMonitor, 0, len(monitorArray))
	for _, mon := range monitorArray {
		simp := monitor.NewSimpleMonitor(mon)
		if opts.Globals.TestMode {
			simp.LastScanned = maxTestingBlock
		}
		results = append(results, simp)
		mon.Close()
	}

	// TODO: Fix export without arrays
	return globals.RenderSlice(&opts.Globals, results)
}
