package exportPkg

import (
	"context"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) readWithdrawals(
	mon *monitor.Monitor,
	filter *filter.AppearanceFilter,
	errorChan chan error,
) ([]*types.SimpleWithdrawal, error) {
	var cnt int
	var err error
	var appMap map[types.SimpleAppearance]*types.SimpleBlock[string]
	if appMap, cnt, err = monitor.AsMap[types.SimpleBlock[string]](mon, filter); err != nil {
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

	iterFunc := func(app types.SimpleAppearance, value *types.SimpleBlock[string]) error {
		var block types.SimpleBlock[string]
		if block, err = opts.Conn.GetBlockHeaderByNumber(uint64(app.BlockNumber)); err != nil {
			return err
		}

		withdrawals := make([]types.SimpleWithdrawal, 0, 16)
		for _, w := range block.Withdrawals {
			if w.Address == mon.Address {
				withdrawals = append(withdrawals, w)
			}
		}
		if len(withdrawals) > 0 {
			block.Withdrawals = withdrawals
			*value = block
		}

		bar.Tick()
		return nil
	}

	errChan := make(chan error)
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()
	go utils.IterateOverMap(ctx, errChan, appMap, iterFunc)
	if stepErr := <-errChan; stepErr != nil {
		return nil, stepErr
	} else {
		bar.Finish(true)
	}

	// Sort the items back into an ordered array by block number
	items := make([]*types.SimpleWithdrawal, 0, len(appMap))
	for _, block := range appMap {
		for _, with := range block.Withdrawals {
			items = append(items, &with)
		}
	}
	sort.Slice(items, func(i, j int) bool {
		if opts.Reversed {
			i, j = j, i
		}
		return items[i].BlockNumber < items[j].BlockNumber
	})

	// Return the array of items
	return items, nil
}
