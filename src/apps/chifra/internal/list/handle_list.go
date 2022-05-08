// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package listPkg

import (
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
)

type SimpleAppearance struct {
	Address          string `json:"address"`
	BlockNumber      uint32 `json:"blockNumber"`
	TransactionIndex uint32 `json:"transactionIndex"`
}

func (opts *ListOptions) HandleListAppearances(monitorArray []monitor.Monitor) error {
	for _, mon := range monitorArray {
		count := mon.Count()
		apps := make([]index.AppearanceRecord, count, count)
		err := mon.ReadAppearances(&apps)
		if err != nil {
			return err
		}
		if len(apps) == 0 {
			fmt.Println("No appearances found for", mon.GetAddrStr())
			return nil
		}

		sort.Slice(apps, func(i, j int) bool {
			si := uint64(apps[i].BlockNumber)
			si = (si << 32) + uint64(apps[i].TransactionId)
			sj := uint64(apps[j].BlockNumber)
			sj = (sj << 32) + uint64(apps[j].TransactionId)
			return si < sj
		})

		exportRange := cache.FileRange{First: opts.FirstBlock, Last: opts.LastBlock}
		results := make([]SimpleAppearance, 0, mon.Count())
		for _, app := range apps {
			appRange := cache.FileRange{First: uint64(app.BlockNumber), Last: uint64(app.BlockNumber)}
			if appRange.Intersects(exportRange) {
				var s SimpleAppearance
				s.Address = mon.GetAddrStr()
				s.BlockNumber = app.BlockNumber
				s.TransactionIndex = app.TransactionId
				results = append(results, s)
			}
		}

		// TODO: Fix export without arrays
		b := make([]interface{}, len(results))
		for i := range results {
			b[i] = results[i]
		}
		err = opts.Globals.OutputArray(b)
		if err != nil {
			return err
		}
	}
	return nil
}
