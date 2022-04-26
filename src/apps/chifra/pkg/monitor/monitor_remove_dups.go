package monitor

import (
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
)

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

func (mon *Monitor) RemoveDups() (uint32, uint32, error) {
	if mon.Count() == 0 {
		return 0, 0, nil
	}

	defer mon.Close()

	apps := make([]index.AppearanceRecord, mon.Count())
	err := mon.ReadAppearances(&apps)
	if err != nil {
		return mon.Count(), mon.Count(), err
	}

	sort.Slice(apps, func(i, j int) bool {
		si := uint64(apps[i].BlockNumber)
		si = (si << 32) + uint64(apps[i].TransactionId)
		sj := uint64(apps[j].BlockNumber)
		sj = (sj << 32) + uint64(apps[j].TransactionId)
		return si < sj
	})

	cntBefore := mon.Count()
	cntAfter := cntBefore

	var prev index.AppearanceRecord
	deDupped := make([]index.AppearanceRecord, 0, mon.Count())
	for i, app := range apps {
		if i == 0 || (prev.BlockNumber != app.BlockNumber || prev.TransactionId != app.TransactionId) {
			deDupped = append(deDupped, app)
		}
		prev = app
	}

	if len(apps) != len(deDupped) {
		mon.Close() // so when we open it, it gets replaced
		cntAfter, err = mon.WriteAppearances(deDupped, os.O_WRONLY|os.O_CREATE|os.O_TRUNC)
		if err != nil {
			return cntBefore, cntAfter, err
		}
		mon.WriteMonHeader(mon.Deleted, mon.LastScanned)
	}

	return cntBefore, cntAfter, err
}
