// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/walk"
)

// HandleList handles the chifra monitors --list command.
func (opts *MonitorsOptions) HandleList(rCtx *output.RenderCtx) error {
	testMode := opts.Globals.TestMode
	if testMode {
		logger.Info("chifra monitors --list not tested")
		return nil
	}

	chain := opts.Globals.Chain
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		vFunc := func(fn string, vP any) (bool, error) {
			_ = vP
			_, name := filepath.Split(fn)
			incStaged := opts.Staged
			isStaging := strings.Contains(fn, "staging")
			isMonitor := strings.HasSuffix(name, ".mon.bin")
			include := isMonitor && (incStaged || !isStaging)
			if include {
				if addr, err := base.AddressFromPath(fn, ".mon.bin"); err == nil && !addr.IsZero() {
					s := types.Monitor{
						Address:  addr,
						NRecords: (file.FileSize(fn) / 8) - 1, // two 32 bit integers and a 32 bit header
						FileSize: file.FileSize(fn),
						IsStaged: isStaging,
					}
					s.IsEmpty = s.NRecords == 0
					if opts.Globals.Verbose {
						var mon monitor.Monitor
						mon.Address = addr
						mon.Staged = isStaging
						_ = mon.ReadMonitorHeader()
						mon.Close()
						s.LastScanned = mon.LastScanned
						s.Deleted = mon.Deleted
					}
					modelChan <- &s
				}
			}
			return true, nil
		}

		path := filepath.Join(config.PathToCache(chain), "monitors")
		_ = walk.ForEveryFileInFolder(path, vFunc, errorChan)
	}

	extraOpts := map[string]any{
		"list": true,
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
