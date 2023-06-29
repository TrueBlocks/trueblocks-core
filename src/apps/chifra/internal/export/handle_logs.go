// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"fmt"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ExportOptions) HandleLogs(monitorArray []monitor.Monitor) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	exportRange := base.FileRange{First: opts.FirstBlock, Last: opts.LastBlock}
	nExported := uint64(0)
	nSeen := int64(-1)

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawLog], errorChan chan error) {
		visitAppearance := func(app *types.SimpleAppearance) error {
			raw := types.RawAppearance{
				Address:          app.Address.Hex(),
				BlockNumber:      uint32(app.BlockNumber),
				TransactionIndex: uint32(app.TransactionIndex),
			}
			if tx, err := rpcClient.GetTransactionByAppearance(chain, &raw, false); err != nil {
				errorChan <- err
				return nil
			} else {
				matchesFourByte := len(opts.Fourbytes) == 0 // either there is no four bytes...
				for _, fb := range opts.Fourbytes {
					if strings.HasPrefix(tx.Input, fb) {
						matchesFourByte = true
					}
				}
				if matchesFourByte {
					for _, log := range tx.Receipt.Logs {
						log := log
						if opts.isRelevant(monitorArray, log) {
							if opts.matchesFilter(&log) {
								modelChan <- &log
							}
						}
					}
				}
				return nil
			}
		}

		for _, mon := range monitorArray {
			count := mon.Count()
			apps := make([]index.AppearanceRecord, count)
			err := mon.ReadAppearances(&apps)
			if err != nil {
				errorChan <- err
				return
			}
			if len(apps) == 0 {
				errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
				return
			}

			sort.Slice(apps, func(i, j int) bool {
				si := uint64(apps[i].BlockNumber)
				si = (si << 32) + uint64(apps[i].TransactionId)
				sj := uint64(apps[j].BlockNumber)
				sj = (sj << 32) + uint64(apps[j].TransactionId)
				return si < sj
			})

			currentBn := uint32(0)
			currentTs := int64(0)
			for i, app := range apps {
				nSeen++
				appRange := base.FileRange{First: uint64(app.BlockNumber), Last: uint64(app.BlockNumber)}
				if appRange.Intersects(exportRange) {
					if nSeen < int64(opts.FirstRecord) {
						logger.Progress(!testMode && true, "Skipping:", nExported, opts.FirstRecord)
						continue
					} else if opts.IsMax(nExported) {
						logger.Progress(!testMode && true, "Quitting:", nExported, opts.FirstRecord)
						return
					}
					nExported++

					logger.Progress(!testMode && nSeen%723 == 0, "Processing: ", mon.Address.Hex(), " ", app.BlockNumber, ".", app.TransactionId)

					logger.Progress(!testMode && nSeen%723 == 0, "Processing: ", mon.Address.Hex(), " ", app.BlockNumber, ".", app.TransactionId)
					if app.BlockNumber != currentBn || app.BlockNumber == 0 {
						currentTs, _ = tslib.FromBnToTs(chain, uint64(app.BlockNumber))
					}
					currentBn = app.BlockNumber

					if err = visitAppearance(&types.SimpleAppearance{
						Address:          mon.Address,
						BlockNumber:      app.BlockNumber,
						TransactionIndex: app.TransactionId,
						Timestamp:        currentTs,
					}); err != nil {
						errorChan <- err
						return
					}

				} else {
					logger.Progress(!testMode && i%100 == 0, "Skipping:", app)
				}
			}
		}
	}

	extra := map[string]interface{}{
		"testMode": testMode,
		"export":   true,
	}

	if opts.Globals.Verbose || opts.Globals.Format == "json" {
		parts := names.Custom | names.Prefund | names.Regular
		namesMap, err := names.LoadNamesMap(chain, parts, nil)
		if err != nil {
			return err
		}
		extra["namesMap"] = namesMap
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

func (opts *ExportOptions) isRelevant(monitorArray []monitor.Monitor, log types.SimpleLog) bool {
	if !opts.Relevant {
		return true
	}

	haystack := ""
	for _, topic := range log.Topics {
		haystack += strings.Replace(topic.Hex(), "0x", "", -1)
	}
	haystack += log.Data
	for _, mon := range monitorArray {
		if strings.Contains(strings.ToLower(haystack), mon.Address.Hex()[2:]) {
			return true
		}
	}
	return false
}

func (opts *ExportOptions) matchesFilter(log *types.SimpleLog) bool {
	return opts.matchesTopic(log) && opts.matchesEmitter(log)
}

func (opts *ExportOptions) matchesEmitter(log *types.SimpleLog) bool {
	for _, e := range opts.Emitter {
		if e == log.Address.Hex() {
			return true
		}
	}
	return len(opts.Emitter) == 0
}

func (opts *ExportOptions) matchesTopic(log *types.SimpleLog) bool {
	for _, t := range opts.Topics {
		if t == log.Topics[0].Hex() {
			return true
		}
	}
	return len(opts.Topics) == 0
}
