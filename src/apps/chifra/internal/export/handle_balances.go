package exportPkg

import (
	"context"
	"fmt"
	"math/big"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
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
	exportRange := base.FileRange{First: opts.FirstBlock, Last: opts.LastBlock}
	nExported := uint64(0)
	nSeen := int64(-1)

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
			var bar = logger.NewBar(mon.Count())
			if theMap, cnt, err := monitor.ReadAppearancesToMap[BalanceHistory](&mon); err != nil {
				errorChan <- err
				return
			} else if cnt == 0 {
				errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex()) // continue even on errors
			} else {
				// At this point, theMap does not contain any data, just keys (appearances) with empty BalanceHistories. Fill the histories in.
				errorChan2 := make(chan error)
				utils.IterateOverMap(ctx, errorChan2, theMap, func(key index.AppearanceRecord, value *BalanceHistory) error {
					if b, err := rpcClient.GetBalanceAt(chain, mon.Address, uint64(key.BlockNumber)); err != nil {
						errorChan <- err
						return err
					} else {
						*value = BalanceHistory{
							Address:          mon.Address,
							BlockNumber:      uint64(key.BlockNumber),
							TransactionIndex: uint64(key.TransactionId),
							Balance:          b,
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

				currentBn := base.Blknum(0)
				currentTs := base.Timestamp(0)
				prevBal, _ := rpcClient.GetBalanceAt(chain, mon.Address, opts.FirstBlock)
				for _, h := range histories {
					nSeen++
					appRange := base.FileRange{First: h.BlockNumber, Last: h.BlockNumber}
					if appRange.Intersects(exportRange) {
						if nSeen < int64(opts.FirstRecord) {
							logger.Progress(!testMode && true, "Skipping:", nExported, opts.FirstRecord)
							continue
						} else if opts.IsMax(nExported) {
							logger.Progress(!testMode && true, "Quitting:", nExported, opts.FirstRecord)
							return
						}
						nExported++

						logger.Progress(!testMode && nSeen%723 == 0, "Processing: ", mon.Address.Hex(), " ", h.BlockNumber, ".", h.TransactionIndex)
						if h.BlockNumber != currentBn || h.BlockNumber == 0 {
							currentTs, _ = tslib.FromBnToTs(chain, uint64(h.BlockNumber))
						}
						currentBn = h.BlockNumber
						h.Timestamp = currentTs
						h.Previous = prevBal
						if err := visitAppearance(&h); err != nil {
							errorChan <- err
							return
						}
					}
				}
			}
			if !utils.IsTerminal() {
				bar.Finish()
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
