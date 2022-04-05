// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package listPkg

import (
	"fmt"
	"net/http"
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
)

type SimpleAppearance struct {
	Address          string `json:"address"`
	BlockNumber      uint32 `json:"blockNumber"`
	TransactionIndex uint32 `json:"transactionIndex"`
}

func (opts *ListOptions) HandleListAppearances(monitorArray []monitor.Monitor) error {
	// theWriter := csv.NewWriter(os.Stdout)
	// theWriter.Comma = 0x9
	for _, mon := range monitorArray {
		apps := make([]index.AppearanceRecord, mon.Count, mon.Count)
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

		results := make([]SimpleAppearance, 0, mon.Count)
		for _, app := range apps {
			// range1 := cache.FileRange{First: opts.FirstBlock, Last: opts.LastBlock}
			// range2 := cache.FileRange{First: uint64(app.BlockNumber), Last: uint64(app.BlockNumber)}
			// if Intersects(range1, range2) {
			var s SimpleAppearance
			s.Address = mon.GetAddrStr()
			s.BlockNumber = app.BlockNumber
			s.TransactionIndex = app.TransactionId
			results = append(results, s)
			// }
		}

		// TODO: Fix export without arrays
		if opts.Globals.ApiMode {
			opts.Globals.Respond(opts.Globals.Writer, http.StatusOK, results)

		} else {
			// fmt.Println("SOMETHING IS BROKEN HERE")
			// fmt.Println("Size: ", len(results))
			// fmt.Println("Results: ", results)
			err := opts.Globals.Output(os.Stdout, opts.Globals.Format, results)
			if err != nil {
				logger.Log(logger.Error, err)
			}
		}
	}
	return nil
}
