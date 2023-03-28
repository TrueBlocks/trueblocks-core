// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package listPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ListOptions) HandleListCount(monitorArray []monitor.Monitor) error {
	testMode := opts.Globals.TestMode

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawMonitor], errorChan chan error) {
		for _, mon := range monitorArray {
			if !opts.NoZero || mon.Count() > 0 {
				s := types.SimpleMonitor{
					Address:     mon.Address.Hex(),
					NRecords:    int(mon.Count()),
					FileSize:    file.FileSize(mon.Path()),
					LastScanned: mon.Header.LastScanned,
				}
				if testMode {
					s.NRecords = 1001001
					s.FileSize = 1001001
					s.LastScanned = maxTestingBlock
				}
				modelChan <- &s
			}
			mon.Close()
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
