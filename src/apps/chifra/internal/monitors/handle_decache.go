// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/user"
)

// HandleDecache
func (opts *MonitorsOptions) HandleDecache() error {
	if opts.Globals.TestMode {
		logger.Warn("Decache option not tested.")
		return nil
	}

	for _, addr := range opts.Addrs {
		if !base.IsValidAddress(addr) {
			continue
		}

		m := monitor.NewMonitor(opts.Globals.Chain, addr, false)
		if !user.QueryUser(getWarning(addr, m.Count()), "Not decaching") {
			continue
		}

		if !file.FileExists(m.Path()) {
			msg := "No monitor was found for address " + addr + "."
			logger.Warn(msg)
			continue

		} else {
			logger.Info("Decaching", addr)

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
			err := m.Decache(opts.Globals.Chain, processorFunc)
			if err != nil {
				return err
			}
			logger.Info(itemsRemoved, "items totaling", bytesRemoved, "bytes were removed from the cache.", strings.Repeat(" ", 60))

			// We've visited them all, so delete the monitor itself
			m.Delete()
			logger.Info(("Monitor " + addr + " was deleted but not removed."))
			wasRemoved, err := m.Remove()
			if !wasRemoved || err != nil {
				logger.Info(("Monitor for " + addr + " was not removed (" + err.Error() + ")"))
			} else {
				logger.Info(("Monitor for " + addr + " was permanently removed."))
			}
		}
	}

	return nil
}

func getWarning(addr string, count uint32) string {
	var warning = strings.Replace("Are sure you want to decache {0}{1} (Yy)?", "{0}", addr, -1)
	if count > 5000 {
		return strings.Replace(strings.Replace(warning, "{1}", ". It may take a long time to process {2} records.", -1), "{2}", fmt.Sprintf("%d", count), -1)
	}
	return strings.Replace(warning, "{1}", "", -1)
}
