package exportPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ExportOptions) readWithdrawals(
	addrArray []base.Address,
	mon *monitor.Monitor,
	filter *filter.AppearanceFilter,
	errorChan chan error,
) ([]*types.SimpleWithdrawal, error) {

	// var cnt int
	// var err error
	// var blockMap map[types.SimpleAppearance]*types.SimpleBlock[string]

	// filter.BlocksOnly = true
	// if blockMap, cnt, err = monitor.ReadAppearancesToMap[types.SimpleBlock[string]](mon, filter); err != nil {
	// 	errorChan <- err
	// 	return nil, err
	// }

	// if opts.NoZero && cnt == 0 {
	// 	errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
	// 	return nil, nil
	// }

	// bar := logger.NewBar(logger.BarOptions{
	// 	Prefix:  mon.Address.Hex(),
	// 	Enabled: !opts.Globals.TestMode,
	// 	Total:   mon.Count(),
	// })

	// if err := opts.Conn.ReadTransactions(blockMap, bar, false /* readTraces */); err != nil { // calls IterateOverMap
	// 	return nil, err
	// }

	items := make([]*types.SimpleWithdrawal, 0, 12) // len(blockMap))
	// for _, blk := range blockMap {
	// 	for _, wd := range blk.Withdrawals {
	// 		items = append(items, &wd)
	// 	}
	// }
	// sort.Slice(items, func(i, j int) bool {
	// 	if opts.Reversed {
	// 		i, j = j, i
	// 	}
	// 	itemI := items[i]
	// 	itemJ := items[j]
	// 	return itemI.Index == itemJ.Index
	// })

	return items, nil
}
