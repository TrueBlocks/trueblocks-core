// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"fmt"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/usage"
)

// HandleDecache handles the command chifra monitors --decache
func (opts *ExportOptions) HandleDecache(monitorArray []monitor.Monitor) error {
	chain := opts.Globals.Chain

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		for _, mon := range monitorArray {
			mon := mon
			if !opts.Globals.IsApiMode() && !usage.QueryUser(getWarning(mon.Address.Hex(), mon.Count()), "Not decaching") {
				continue
			}
			logger.Info("Decaching", mon.Address.Hex())
			itemsRemoved := int64(0)
			bytesRemoved := int64(0)
			processorFunc := func(fileName string) bool {
				itemsRemoved++
				bytesRemoved += file.FileSize(fileName)
				os.Remove(fileName)
				if opts.Globals.Verbose {
					logger.Info("Removed ", itemsRemoved, " items and ", bytesRemoved, " bytes.", fileName)
				}
				path, _ := filepath.Split(fileName)
				if empty, _ := file.IsFolderEmpty(path); empty {
					os.RemoveAll(path)
					if opts.Globals.Verbose {
						logger.Info("Empty folder", path, "was removed.")
					}
				}
				return true
			}

			// Visits every item in the cache related to this monitor and calls into `processorFunc`
			if err := mon.Decache(chain, processorFunc); err != nil {
				errorChan <- err
			} else {
				logger.Info(itemsRemoved, "items totaling", bytesRemoved, "bytes were removed from the cache.", strings.Repeat(" ", 60))

				// We've visited them all, so delete the monitor itself
				mon.Delete()
				msg := fmt.Sprintf("Monitor %s was deleted but not removed.", mon.Address.Hex())
				if wasRemoved, err := mon.Remove(); !wasRemoved || err != nil {
					msg += fmt.Sprintf(" Monitor for %s was not removed (%s).", mon.Address.Hex(), err.Error())
				} else {
					msg += fmt.Sprintf(" Monitor for %s  was permanently removed.", mon.Address.Hex())
				}
				s := types.SimpleMessage{
					Msg: msg,
				}
				modelChan <- &s
			}
		}
	}

	opts.Globals.NoHeader = true
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

func getWarning(addr string, count int64) string {
	var warning = strings.Replace("Are sure you want to decache {0}{1} (Yy)?", "{0}", addr, -1)
	if count > 5000 {
		return strings.Replace(strings.Replace(warning, "{1}", ". It may take a long time to process {2} records.", -1), "{2}", fmt.Sprintf("%d", count), -1)
	}
	return strings.Replace(warning, "{1}", "", -1)
}
