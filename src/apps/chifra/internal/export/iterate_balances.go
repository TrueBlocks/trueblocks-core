package exportPkg

import (
	"context"
	"fmt"
	"math/big"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) readBalances(
	mon *monitor.Monitor,
	filter *filter.AppearanceFilter,
	errorChan chan error,
) ([]*types.SimpleToken, error) {

	var cnt int
	var err error
	var txMap map[types.SimpleAppearance]*types.SimpleToken

	if txMap, cnt, err = monitor.ReadAppearancesToMap[types.SimpleToken](mon, filter); err != nil {
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

	iterFunc := func(app types.SimpleAppearance, value *types.SimpleToken) error {
		var balance *big.Int
		if balance, err = opts.Conn.GetBalanceByAppearance(mon.Address, &app); err != nil {
			return err
		}

		value.Address = base.FAKE_ETH_ADDRESS
		value.Holder = mon.Address
		value.BlockNumber = uint64(app.BlockNumber)
		value.TransactionIndex = uint64(app.TransactionIndex)
		value.Balance = *balance
		value.Timestamp = app.Timestamp
		bar.Tick()

		return nil
	}

	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()

	errChan := make(chan error)
	go utils.IterateOverMap(ctx, errChan, txMap, iterFunc)
	if stepErr := <-errChan; stepErr != nil {
		return nil, stepErr
	} else {
		bar.Finish(true)
	}

	// Sort the items back into an ordered array by block number
	items := make([]*types.SimpleToken, 0, len(txMap))
	for _, tx := range txMap {
		items = append(items, tx)
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
