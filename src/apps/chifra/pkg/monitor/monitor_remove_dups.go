package monitor

import (
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
)

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

func (mon *Monitor) RemoveDups() (cntBefore uint32, cntAfter uint32, err error) {
	defer mon.Close()

	cntBefore = mon.Count()
	cntAfter = cntBefore
	if cntBefore > 0 {
		apps := make([]index.AppearanceRecord, mon.Count())
		deDupped := make([]index.AppearanceRecord, 0, mon.Count())
		err = mon.ReadAppearances(&apps)
		if err != nil {
			return
		}

		sort.Slice(apps, func(i, j int) bool {
			si := uint64(apps[i].BlockNumber)
			si = (si << 32) + uint64(apps[i].TransactionId)
			sj := uint64(apps[j].BlockNumber)
			sj = (sj << 32) + uint64(apps[j].TransactionId)
			return si < sj
		})

		deDupped = append(deDupped, apps[0])
		prev := apps[0]
		for i, app := range apps {
			if i > 0 {
				if prev.BlockNumber != app.BlockNumber || prev.TransactionId != app.TransactionId {
					deDupped = append(deDupped, app)
					prev = app
				}
			}
		}

		if len(apps) != len(deDupped) {
			mon.Close() // so when we open it, it gets replaced
			_, err = mon.WriteAppearances(deDupped, os.O_WRONLY|os.O_CREATE|os.O_TRUNC)
			if err != nil {
				return
			}
			cntAfter = uint32(len(deDupped))
		}
	}

	return
}
