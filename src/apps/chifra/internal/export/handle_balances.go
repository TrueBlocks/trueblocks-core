package exportPkg

import (
	"context"
	"fmt"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common"
)

type BalanceHistory struct {
	App       *types.SimpleAppearance
	Timestamp int64
	Previous  *big.Int
}

func (opts *ExportOptions) HandleBalances(monitorArray []monitor.Monitor) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	sortBy := monitor.Sorted
	if opts.Reversed {
		sortBy = monitor.Reversed
	}
	exportRange := base.FileRange{First: opts.FirstBlock, Last: opts.LastBlock}
	nExported := uint64(0)
	nSeen := int64(-1)

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawTokenBalance], errorChan chan error) {
		visitAppearance := func(balCtx *BalanceHistory) error {
			app := balCtx.App
			a := common.HexToAddress(app.Address.Hex())
			if bb, err := rpcClient.GetBalanceAt(chain, a, uint64(app.BlockNumber)); err != nil {
				errorChan <- err
			} else {
				if balCtx.Previous.Cmp(bb) != 0 {
					bal := types.SimpleTokenBalance{
						Holder:           app.Address,
						BlockNumber:      uint64(app.BlockNumber),
						TransactionIndex: uint64(app.TransactionIndex),
						Balance:          *bb,
						Timestamp:        balCtx.Timestamp,
					}
					modelChan <- &bal
					*balCtx.Previous = *bb
				}
			}
			return nil
		}

		for _, mon := range monitorArray {
			if apps, cnt, err := mon.ReadAppearances2(sortBy); err != nil {
				errorChan <- err
				return
			} else if cnt == 0 {
				errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
				return
			} else {
				currentBn := uint32(0)
				currentTs := int64(0)
				prevBal := big.NewInt(0)
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
						if app.BlockNumber != currentBn || app.BlockNumber == 0 {
							currentTs, _ = tslib.FromBnToTs(chain, uint64(app.BlockNumber))
						}
						currentBn = app.BlockNumber

						s := &types.SimpleAppearance{
							Address:          mon.Address,
							BlockNumber:      app.BlockNumber,
							TransactionIndex: app.TransactionId,
							Timestamp:        currentTs,
						}
						if err := visitAppearance(&BalanceHistory{
							App:       s,
							Timestamp: currentTs,
							Previous:  prevBal,
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
	}

	nameParts := names.Custom | names.Prefund | names.Regular
	namesMap, err := names.LoadNamesMap(chain, nameParts, nil)
	if err != nil {
		return err
	}

	extra := map[string]interface{}{
		"testMode": testMode,
		"namesMap": namesMap,
		"parts":    []string{"blockNumber", "holder", "decimals", "balance", "units"},
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
