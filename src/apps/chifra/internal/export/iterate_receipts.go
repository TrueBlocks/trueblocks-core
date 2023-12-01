package exportPkg

import (
	"context"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) readReceipts(
	addrArray []base.Address,
	mon *monitor.Monitor,
	filter *filter.AppearanceFilter,
	errorChan chan error,
	abiCache *articulate.AbiCache,
) ([]*types.SimpleReceipt, error) {
	var cnt int
	var err error
	var appMap map[types.SimpleAppearance]*types.SimpleTransaction
	if appMap, cnt, err = monitor.AsMap[types.SimpleTransaction](mon, filter); err != nil {
		errorChan <- err
		return nil, err
	} else if opts.NoZero && cnt == 0 {
		errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
		return nil, nil
	}

	bar := logger.NewBar(logger.BarOptions{
		Prefix:  mon.Address.Hex(),
		Enabled: !opts.Globals.TestMode,
		Total:   mon.Count(),
	})

	iterFunc := func(app types.SimpleAppearance, value *types.SimpleTransaction) error {
		if tx, err := opts.Conn.GetTransactionByAppearance(&app, false); err != nil {
			return err
		} else {
			passes, _ := filter.ApplyTxFilters(tx)
			if passes {
				*value = *tx
			}
			if bar != nil {
				bar.Tick()
			}
			return nil
		}
	}

	// Set up and interate over the map calling iterFunc for each appearance
	iterCtx, iterCancel := context.WithCancel(context.Background())
	defer iterCancel()
	errChan := make(chan error)
	go utils.IterateOverMap(iterCtx, errChan, appMap, iterFunc)
	if stepErr := <-errChan; stepErr != nil {
		errorChan <- stepErr
		return nil, stepErr
	}

	items := make([]*types.SimpleReceipt, 0, len(appMap))
	for _, tx := range appMap {
		if tx.Receipt == nil {
			continue
		}
		filteredLogs := make([]types.SimpleLog, 0, len(tx.Receipt.Logs))
		for _, log := range tx.Receipt.Logs {
			log := log
			if filter.ApplyLogFilter(&log, addrArray) && opts.matchesFilter(&log) {
				if opts.Articulate {
					if err := abiCache.ArticulateLog(&log); err != nil {
						errorChan <- fmt.Errorf("error articulating log: %v", err)
					}
				}
				filteredLogs = append(filteredLogs, log)
			}
		}
		tx.Receipt.Logs = filteredLogs
		items = append(items, tx.Receipt)
	}
	sort.Slice(items, func(i, j int) bool {
		if opts.Reversed {
			i, j = j, i
		}
		itemI := items[i]
		itemJ := items[j]
		if itemI.BlockNumber == itemJ.BlockNumber {
			return itemI.TransactionIndex < itemJ.TransactionIndex
		}
		return itemI.BlockNumber < itemJ.BlockNumber
	})

	return items, nil
}
