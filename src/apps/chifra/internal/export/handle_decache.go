// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"fmt"
	"os"
	"path"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/decache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/usage"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// HandleDecache handles the command chifra monitors --decache
func (opts *ExportOptions) HandleDecache(monitorArray []monitor.Monitor) error {
	chain := opts.Globals.Chain
	silent := opts.Globals.TestMode || len(opts.Globals.File) > 0

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		for _, mon := range monitorArray {
			mon := mon
			if !opts.Globals.IsApiMode() && !usage.QueryUser(getWarning(mon.Address.Hex(), mon.Count()), "Not decaching") {
				continue
			}

			if apps, cnt, err := mon.ReadAndFilterAppearances(monitor.NewEmptyFilter(chain)); err != nil {
				errorChan <- err
				continue

			} else if cnt == 0 {
				continue

			} else {
				caches := []walk.CacheType{
					walk.Cache_Statements,
					walk.Cache_Traces,
					walk.Cache_Transactions}

				for _, cache := range caches {
					itemsToRemove, err := decache.LocationsFromAddrAppsAndCacheType(opts.Conn, mon.Address, apps, cache)
					if err != nil {
						errorChan <- err
						continue
					}

					if msg, err := decache.Decache(opts.Conn, itemsToRemove, silent, mon.Address.Hex()); err != nil {
						errorChan <- err

					} else {
						logger.Info(msg)

						// remove the ABI if we can find it
						abiPath := path.Join(walk.CacheTypeToFolder[walk.Cache_Abis], mon.Address.Hex()+".json")
						if file.FileExists(abiPath) {
							os.Remove(abiPath)
							logger.Info("Abi " + abiPath + " file removed.")
						}

						// delete the monitor
						if mon.IsOpen() {
							mon.Close()
						}
						mon.Delete()
						msg := fmt.Sprintf("Monitor %s was deleted but not removed.", mon.Address.Hex())
						if wasRemoved, err := mon.Remove(); !wasRemoved || err != nil {
							msg += fmt.Sprintf(" Monitor for %s was not removed (%s).", mon.Address.Hex(), err.Error())
						} else {
							msg += fmt.Sprintf(" Monitor for %s  was permanently removed.", mon.Address.Hex())
						}

						modelChan <- &types.SimpleMessage{
							Msg: msg,
						}
					}
				}
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
