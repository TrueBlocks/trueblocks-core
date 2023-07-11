package exportPkg

import (
	"context"
	"fmt"
	"math/big"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) HandleBalances(monitorArray []monitor.Monitor) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	filter := monitor.NewFilter(
		chain,
		true,
		opts.Reversed,
		!testMode,
		base.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
		base.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
	)

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawTokenBalance], errorChan chan error) {
		for _, mon := range monitorArray {
			if theMap, cnt, err := monitor.ReadAppearancesToMap[types.SimpleTokenBalance](&mon, filter); err != nil {
				errorChan <- err
				return
			} else if !opts.NoZero || cnt > 0 {
				if balances, err := opts.readBalances(&mon, theMap); err != nil {
					errorChan <- err
					continue
				} else {
					prevBalance, _ := rpcClient.GetBalanceAt(chain, mon.Address, filter.GetOuterBounds().First)
					for index, bal := range balances {
						bal := bal
						bal.PriorBalance = *prevBalance
						if opts.Globals.Verbose || index == 0 || bal.PriorBalance.Cmp(&bal.Balance) != 0 {
							bal.Diff = *big.NewInt(0).Sub(&bal.Balance, &bal.PriorBalance)
							modelChan <- bal
							prevBalance = &bal.Balance
						}
					}
				}
			} else {
				errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
				continue
			}
		}
	}

	nameParts := names.Custom | names.Prefund | names.Regular
	namesMap, err := names.LoadNamesMap(chain, nameParts, nil)
	if err != nil {
		return err
	}

	extra := map[string]interface{}{
		"testMode": testMode,
		"namesMap": namesMap,
		"parts":    []string{"blockNumber", "date", "holder", "balance", "diff", "units"},
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

func (opts *ExportOptions) readBalances(mon *monitor.Monitor, theMap map[types.SimpleAppearance]*types.SimpleTokenBalance) ([]*types.SimpleTokenBalance, error) {
	chain := opts.Globals.Chain
	showProgress := !opts.Globals.TestMode
	var bar = logger.NewBar(mon.Address.Hex(), showProgress, mon.Count())

	// This is called concurrently, once for each appearance
	iterFunc := func(key types.SimpleAppearance, value *types.SimpleTokenBalance) error {
		if b, err := rpcClient.GetBalanceAt(chain, mon.Address, uint64(key.BlockNumber)); err != nil {
			return err
		} else {
			value.Address = base.FAKE_ETH_ADDRESS
			value.Holder = mon.Address
			value.BlockNumber = uint64(key.BlockNumber)
			value.TransactionIndex = uint64(key.TransactionIndex)
			value.Balance = *b
			value.Timestamp = key.Timestamp
			bar.Tick()
			return nil
		}
	}

	// Set up and interate over the map calling iterFunc for each appearance
	errChan := make(chan error)
	ctx := context.Background()
	utils.IterateOverMap(ctx, errChan, theMap, iterFunc)
	if stepErr := <-errChan; stepErr != nil {
		return nil, stepErr
	} else {
		bar.Finish(true)
	}

	// Sort the balances back into an ordered array by block number
	balances := make([]*types.SimpleTokenBalance, 0, len(theMap))
	for _, v := range theMap {
		balances = append(balances, v)
	}
	sort.Slice(balances, func(i, j int) bool {
		return balances[i].BlockNumber < balances[j].BlockNumber
	})

	// Return the array of balances
	return balances, nil
}
