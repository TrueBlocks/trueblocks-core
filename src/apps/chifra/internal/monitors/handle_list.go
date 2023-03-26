// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// HandleList
func (opts *MonitorsOptions) HandleList() error {
	chain := opts.Globals.Chain
	monitorArray := GetAllMonitors(chain)
	objs := []types.SimpleMonitor{}
	for _, mon := range monitorArray {
		s := types.SimpleMonitor{
			Address:     hexutil.Encode(mon.Address.Bytes()),
			NRecords:    int(mon.Count()),
			FileSize:    file.FileSize(mon.Path()),
			LastScanned: mon.Header.LastScanned,
		}
		if len(opts.Addrs) == 0 || InStrArray(s.Address, opts.Addrs) {
			objs = append(objs, s)
		}
	}

	return globals.RenderSlice(&opts.Globals, objs)
}

func GetAllMonitors(chain string) []monitor.Monitor {
	monitorChan := make(chan monitor.Monitor)

	var monitors []monitor.Monitor
	go monitor.ListMonitors(chain, "monitors", monitorChan)

	for result := range monitorChan {
		switch result.Address {
		case monitor.SentinalAddr:
			close(monitorChan)
		default:
			monitors = append(monitors, result)
		}
	}
	return monitors
}

// TODO: Move this to utils package
func InStrArray(item string, items []string) bool {
	for _, i := range items {
		if i == item {
			return true
		}
	}
	return false
}
