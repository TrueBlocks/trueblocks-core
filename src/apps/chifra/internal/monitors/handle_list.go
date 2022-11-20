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

// HandleClean
func (opts *MonitorsOptions) HandleList() error {
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

	objs := []types.SimpleMonitor{}
	for _, mon := range monitors {
		obj := types.SimpleMonitor{
			Address:     hexutil.Encode(mon.Address.Bytes()),
			NRecords:    int(mon.Count()),
			FileSize:    file.FileSize(mon.Path()),
			LastScanned: mon.Header.LastScanned,
		}
		if len(opts.Addrs) == 0 || InStrArray(obj.Address, opts.Addrs) {
			objs = append(objs, obj)
		}
	}

	// TODO: Fix export without arrays
	return globals.RenderSlice(&opts.Globals, objs)
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
