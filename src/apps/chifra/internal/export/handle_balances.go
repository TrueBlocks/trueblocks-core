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

type BalanceHistory struct {
	Address          base.Address
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	Timestamp        base.Timestamp
	Previous         *big.Int
	Balance          *big.Int
	Diff             *big.Int
}

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

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawTokenBalance], errorChan chan error) {
		visitAppearance := func(bal *BalanceHistory) error {
			if opts.Globals.Verbose || bal.Previous.Cmp(bal.Balance) != 0 {
				diff := big.NewInt(0).Sub(bal.Balance, bal.Previous)
				tb := types.SimpleTokenBalance{
					Holder:           bal.Address,
					BlockNumber:      bal.BlockNumber,
					TransactionIndex: bal.TransactionIndex,
					Timestamp:        bal.Timestamp,
					Diff:             *diff,
					Balance:          *bal.Balance,
				}
				modelChan <- &tb
				*bal.Previous = *bal.Balance
			}
			return nil
		}

		for _, mon := range monitorArray {
			filter.Reset()
			var bar = logger.NewBar(mon.Count())
			if theMap, cnt, err := monitor.ReadAppearancesToMap[BalanceHistory](&mon, filter); err != nil {
				errorChan <- err
				return
			} else if !opts.NoZero || cnt > 0 {
				errorChan2 := make(chan error)
				utils.IterateOverMap(ctx, errorChan2, theMap, func(key types.SimpleAppearance, value *BalanceHistory) error {
					if b, err := rpcClient.GetBalanceAt(chain, mon.Address, uint64(key.BlockNumber)); err != nil {
						errorChan <- err
						return err
					} else {
						*value = BalanceHistory{
							Address:          mon.Address,
							BlockNumber:      uint64(key.BlockNumber),
							TransactionIndex: uint64(key.TransactionIndex),
							Balance:          b,
							Timestamp:        key.Timestamp,
						}
						bar.Tick()
						return nil
					}
				})
				if stepErr := <-errorChan2; stepErr != nil {
					cancel()
					errorChan <- stepErr
					return
				}

				histories := make([]BalanceHistory, 0, len(theMap))
				for _, v := range theMap {
					histories = append(histories, *v)
				}
				sort.Slice(histories, func(i, j int) bool {
					return histories[i].BlockNumber < histories[j].BlockNumber
				})

				prevBal, _ := rpcClient.GetBalanceAt(chain, mon.Address, filter.GetOuterBounds().First)
				for _, h := range histories {
					h := h
					h.Previous = prevBal
					if err := visitAppearance(&h); err != nil {
						errorChan <- err
						return
					}
				}
			} else {
				errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
				continue
			}
			bar.Finish(!utils.IsTerminal())
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
