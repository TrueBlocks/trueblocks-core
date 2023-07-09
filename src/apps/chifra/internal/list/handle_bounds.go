package listPkg

import (
	"context"
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ListOptions) HandleBounds(monitorArray []monitor.Monitor) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	filter := monitor.NewFilter(
		chain,
		false, // we will get timestamps by hand below since we're only getting two
		false,
		!testMode,
		base.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
		base.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
	)

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		if len(monitorArray) == 0 {
			errorChan <- errors.New("no monitors found in HandleBounds")
			return
		}

		for _, mon := range monitorArray {
			if apps, cnt, err := mon.ReadAndFilterAppearances(filter); err != nil {
				errorChan <- err
				return
			} else if !opts.NoZero || cnt > 0 {
				firstTs, _ := tslib.FromBnToTs(chain, uint64(apps[0].BlockNumber))
				latestTs, _ := tslib.FromBnToTs(chain, uint64(apps[len(apps)-1].BlockNumber))
				s := simpleBounds{
					Count: uint64(cnt),
					FirstApp: types.RawAppearance{
						Address:          mon.Address.Hex(),
						BlockNumber:      apps[0].BlockNumber,
						TransactionIndex: apps[0].TransactionIndex,
					},
					FirstTs: firstTs,
					LatestApp: types.RawAppearance{
						Address:          mon.Address.Hex(),
						BlockNumber:      apps[len(apps)-1].BlockNumber,
						TransactionIndex: apps[len(apps)-1].TransactionIndex,
					},
					LatestTs: latestTs,
				}
				modelChan <- &s
			} else {
				errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
				continue
			}
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
