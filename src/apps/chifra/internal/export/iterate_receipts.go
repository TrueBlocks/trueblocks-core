package exportPkg

import (
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
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
	var txMap map[types.SimpleAppearance]*types.SimpleTransaction

	if txMap, cnt, err = monitor.ReadAppearancesToMap[types.SimpleTransaction](mon, filter); err != nil {
		errorChan <- err
		return nil, err
	}

	if opts.NoZero && cnt == 0 {
		errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
		return nil, nil
	}

	bar := logger.NewBar(logger.BarOptions{
		Prefix:  mon.Address.Hex(),
		Enabled: !opts.Globals.TestMode && len(opts.Globals.File) == 0,
		Total:   mon.Count(),
	})

	if err := opts.Conn.ReadTransactions(txMap, opts.Fourbytes, bar, false /* readTraces */); err != nil { // calls IterateOverMap
		return nil, err
	}

	items := make([]*types.SimpleReceipt, 0, len(txMap))
	for _, tx := range txMap {
		if tx.Receipt == nil {
			continue
		}
		filteredLogs := make([]types.SimpleLog, 0, len(tx.Receipt.Logs))
		for _, log := range tx.Receipt.Logs {
			log := log
			if log.ContainsAny(addrArray) && opts.matchesFilter(&log) {
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
