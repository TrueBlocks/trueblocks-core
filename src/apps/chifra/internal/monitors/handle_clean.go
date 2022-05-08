// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
)

type CleanReport struct {
	Addr     string `json:"addr"`
	SizeThen uint32 `json:"sizeThen"`
	SizeNow  uint32 `json:"sizeNow"`
	Dups     uint32 `json:"dupsRemoved"`
}

func (opts *MonitorsOptions) HandleClean() error {
	monitorChan := make(chan monitor.Monitor)

	var monitors []monitor.Monitor
	go monitor.ListMonitors(opts.Globals.Chain, "monitors", monitorChan)

	for result := range monitorChan {
		switch result.Address {
		case monitor.SentinalAddr:
			close(monitorChan)
		default:
			monitors = append(monitors, result)
		}
	}

	results := []CleanReport{}
	for _, mon := range monitors {
		if opts.Globals.TestMode {
			addr := mon.GetAddrStr()
			if addr == "0x001d14804b399c6ef80e64576f657660804fec0b" ||
				addr == "0x0029218e1dab069656bfb8a75947825e7989b987" {
				results = append(results, CleanReport{
					Addr:     addr,
					SizeThen: 10,
					SizeNow:  8,
					Dups:     10 - 8,
				})
			}
		} else {
			report := CleanReport{
				Addr: mon.GetAddrStr(),
			}
			var err error
			report.SizeThen, report.SizeNow, err = mon.RemoveDups()
			if err != nil {
				return err
			}

			report.Dups = report.SizeThen - report.SizeNow
			if report.SizeThen > 0 {
				results = append(results, report)
			}
		}
	}

	// TODO: Fix export without arrays
	b := make([]interface{}, len(results))
	for i := range results {
		b[i] = results[i]
	}
	return opts.Globals.OutputArray(b)
}
