// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

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

// HandleClean
func (opts *MonitorsOptions) HandleList() error {
	chain := opts.Globals.Chain
	monitorArray := GetAllMonitors(chain)
	var addrMap = make(map[base.Address]bool, len(opts.Addrs))
	for _, addr := range opts.Addrs {
		addrMap[base.HexToAddress(addr)] = true
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawMonitor], errorChan chan error) {
		for _, mon := range monitorArray {
			if len(opts.Addrs) == 0 || addrMap[mon.Address] {
				s := types.SimpleMonitor{
					Address:     hexutil.Encode(mon.Address.Bytes()),
					NRecords:    int(mon.Count()),
					FileSize:    file.FileSize(mon.Path()),
					LastScanned: mon.Header.LastScanned,
				}
				modelChan <- &s
			}
		}
	}

	return output.StreamMany(ctx, fetchData, output.OutputOptions{
		Writer:     opts.Globals.Writer,
		Chain:      opts.Globals.Chain,
		TestMode:   opts.Globals.TestMode,
		NoHeader:   opts.Globals.NoHeader,
		ShowRaw:    opts.Globals.ShowRaw,
		Verbose:    opts.Globals.Verbose,
		LogLevel:   opts.Globals.LogLevel,
		Format:     opts.Globals.Format,
		OutputFn:   opts.Globals.OutputFn,
		Append:     opts.Globals.Append,
		JsonIndent: "  ",
	})
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
