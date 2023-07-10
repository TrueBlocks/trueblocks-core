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
		visitAppearance := func(index int, bal *types.SimpleTokenBalance) (*big.Int, error) {
			if index == 0 || opts.Globals.Verbose || bal.PriorBalance.Cmp(&bal.Balance) != 0 {
				diff := big.NewInt(0).Sub(&bal.Balance, &bal.PriorBalance)
				tb := types.SimpleTokenBalance{
					Holder:           bal.Address,
					BlockNumber:      bal.BlockNumber,
					TransactionIndex: bal.TransactionIndex,
					Timestamp:        bal.Timestamp,
					Diff:             *diff,
					Balance:          bal.Balance,
				}
				modelChan <- &tb
				bal.PriorBalance = bal.Balance
			}
			return &bal.PriorBalance, nil
		}

		for _, mon := range monitorArray {
			var bar = logger.NewBar(mon.Count())
			if theMap, cnt, err := monitor.ReadAppearancesToMap[types.SimpleTokenBalance](&mon, filter); err != nil {
				errorChan <- err
				return
			} else if !opts.NoZero || cnt > 0 {
				iterFunc := func(key types.SimpleAppearance, value *types.SimpleTokenBalance) error {
					if b, err := rpcClient.GetBalanceAt(chain, mon.Address, uint64(key.BlockNumber)); err != nil {
						errorChan <- err
						return err
					} else {
						value.Address = mon.Address
						value.BlockNumber = uint64(key.BlockNumber)
						value.TransactionIndex = uint64(key.TransactionIndex)
						value.Balance = *b
						value.Timestamp = key.Timestamp
						if !testMode {
							bar.Tick()
						}
						return nil
					}
				}

				errorChan2 := make(chan error)
				utils.IterateOverMap(ctx, errorChan2, theMap, iterFunc)
				if stepErr := <-errorChan2; stepErr != nil {
					errorChan <- stepErr
					return
				}
				if !testMode {
					bar.Finish(true)
				}

				balances := make([]*types.SimpleTokenBalance, 0, len(theMap))
				for _, v := range theMap {
					balances = append(balances, v)
				}
				sort.Slice(balances, func(i, j int) bool {
					return balances[i].BlockNumber < balances[j].BlockNumber
				})

				prevBal, _ := rpcClient.GetBalanceAt(chain, mon.Address, filter.GetOuterBounds().First)
				for i, bal := range balances {
					bal := bal
					bal.PriorBalance = *prevBal
					if prevBal, err = visitAppearance(i, bal); err != nil {
						errorChan <- err
						return
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
