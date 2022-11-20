// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// HandleCrudCommands
//
// [State]     | Delete | Undelete | Remove | Decache                    |
// ------------|--------|------------------------------------------------|
// Not Deleted | Delete	| Error    | Error  | Delete, Remove and Decache |
// Deleted     | Error  | Undelete | Remove | Remove and Decache         |
// ------------|--------|------------------------------------------------|
func (opts *MonitorsOptions) HandleCrudCommands() error {
	for _, addr := range opts.Addrs {
		m := monitor.NewMonitor(opts.Globals.Chain, addr, false)
		if !file.FileExists(m.Path()) {
			return validate.Usage("Monitor not found for address " + addr + ".")

		} else {
			if opts.Decache {
				if opts.Globals.TestMode {
					logger.Log(logger.Info, "Decaching monitor for address ", addr, "not tested.")
					return nil
				}

				itemsRemoved := int64(0)
				bytesRemoved := int64(0)
				processorFunc := func(fileName string) bool {
					if !file.FileExists(fileName) {
						return true // continue processing
					}

					fileNameStr := fileName
					if opts.Globals.TestMode {
						fileNameStr = strings.Replace(fileNameStr, config.GetPathToCache(opts.Globals.Chain), "$cachePath/", -1)
					}

					itemsRemoved++
					bytesRemoved += file.FileSize(fileName)

					os.Remove(fileName)
					if opts.Globals.Verbose {
						logger.Log(logger.Info, fileNameStr, "was removed.")
					}
					path, _ := filepath.Split(fileName)
					if empty, _ := file.IsFolderEmpty(path); empty {
						os.RemoveAll(path)
						if opts.Globals.Verbose {
							logger.Log(logger.Info, "Empty folder", path, "was removed.")
						}
					}

					return true
				}

				// Visits every item in the cache related to this monitor and calls into `processorFunc`
				err := m.Decache(opts.Globals.Chain, processorFunc)
				if err != nil {
					return err
				}
				logger.Log(logger.Info, itemsRemoved, "items totaling", bytesRemoved, "bytes were removed from the cache.")

				// We've visited them all, so delete the monitor itself
				m.Delete()
				logger.Log(logger.Info, ("Monitor " + addr + " was deleted but not removed."))
				wasRemoved, err := m.Remove()
				if !wasRemoved || err != nil {
					logger.Log(logger.Info, ("Monitor for " + addr + " was not removed (" + err.Error() + ")"))
				} else {
					logger.Log(logger.Info, ("Monitor for " + addr + " was permanently removed."))
				}

			} else if opts.Undelete && !m.IsDeleted() {
				return validate.Usage("Monitor for {0} must be deleted before being undeleted.", addr)

			} else {
				if opts.Delete && opts.Remove {
					// do nothing, it will be resolved below...

				} else {
					if opts.Delete && m.IsDeleted() {
						return validate.Usage("Monitor for {0} is already deleted.", addr)

					} else if opts.Remove && !m.IsDeleted() {
						return validate.Usage("Cannot remove a file that has not previously been deleted.")
					}
				}
			}
		}
	}

	if opts.Decache {
		return nil
	}

	for _, addr := range opts.Addrs {
		m := monitor.NewMonitor(opts.Globals.Chain, addr, false)
		if opts.Undelete {
			m.UnDelete()
			logger.Log(logger.Info, ("Monitor " + addr + " was undeleted."))

		} else {
			if opts.Delete {
				m.Delete()
				logger.Log(logger.Info, ("Monitor " + addr + " was deleted but not removed."))
			}

			if opts.Remove {
				wasRemoved, err := m.Remove()
				if !wasRemoved || err != nil {
					logger.Log(logger.Info, ("Monitor for " + addr + " was not removed (" + err.Error() + ")"))
				} else {
					logger.Log(logger.Info, ("Monitor for " + addr + " was permanently removed."))
				}
			}
		}
	}

	return nil
}
