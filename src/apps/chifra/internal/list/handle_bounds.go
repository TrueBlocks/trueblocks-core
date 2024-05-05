package listPkg

import (
	"context"
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ListOptions) HandleBounds(monitorArray []monitor.Monitor) error {
	chain := opts.Globals.Chain
	filter := filter.NewFilter(
		opts.Reversed,
		false,
		[]string{},
		base.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
		base.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
	)

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawBounds], errorChan chan error) {
		if len(monitorArray) == 0 {
			errorChan <- errors.New("no monitors found in HandleBounds")
			return
		}

		for _, mon := range monitorArray {
			if apps, cnt, err := mon.ReadAndFilterAppearances(filter, true /* withCount */); err != nil {
				errorChan <- err
				return
			} else if cnt > 0 {
				firstTs, _ := tslib.FromBnToTs(chain, base.Blknum(apps[0].BlockNumber))
				latestTs, _ := tslib.FromBnToTs(chain, base.Blknum(apps[len(apps)-1].BlockNumber))
				s := types.Bounds{
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
