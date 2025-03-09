// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitor

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (mon *Monitor) RemoveDups() (int64, int64, error) {
	if mon.Count() == 0 {
		return 0, 0, nil
	}
	defer mon.Close()

	if apps, cnt, err := mon.ReadAndFilterAppearances(types.NewEmptyFilter(), true /* withCount */); err != nil {
		return mon.Count(), mon.Count(), err

	} else if cnt == 0 {
		return mon.Count(), mon.Count(), nil

	} else {
		cntBefore := mon.Count()
		cntAfter := cntBefore

		var prev types.AppRecord
		deDupped := make([]types.AppRecord, 0, mon.Count())
		for i, app := range apps {
			iApp := types.AppRecord{
				BlockNumber:      app.BlockNumber,
				TransactionIndex: app.TransactionIndex,
			}
			if i == 0 || (prev.BlockNumber != iApp.BlockNumber || prev.TransactionIndex != iApp.TransactionIndex) {
				deDupped = append(deDupped, iApp)
			}
			prev = iApp
		}

		if len(apps) != len(deDupped) {
			mon.Close() // so when we open it, it gets replaced
			// Very important to note - if you use false for append, the header gets overwritten
			// so ordering matters here and we need to write the header afterwards
			cntAfter, err = mon.WriteAppearances(deDupped, false /* append */)
			if err != nil {
				return cntBefore, cntAfter, err
			}
			_ = mon.WriteMonHeader(mon.Deleted, mon.LastScanned, false /* force */)
		}

		return cntBefore, cntAfter, err
	}
}
