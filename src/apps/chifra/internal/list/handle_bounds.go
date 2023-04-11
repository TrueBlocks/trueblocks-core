package listPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ListOptions) HandleBounds(monitorArray []monitor.Monitor) error {
	chain := opts.Globals.Chain
	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		if len(monitorArray) == 0 {
			errorChan <- errors.New("no monitors found in HandleBounds")
			return
		}

		for _, mon := range monitorArray {
			count := mon.Count()
			apps := make([]index.AppearanceRecord, count)
			if len(apps) == 0 {
				errorChan <- errors.New("no appearances found in monitor for " + mon.Address.Hex())
				continue
			} else if err := mon.ReadAppearances(&apps); err != nil {
				errorChan <- err
				return
			}
			firstTs, _ := tslib.FromBnToTs(chain, uint64(apps[0].BlockNumber))
			lastTs, _ := tslib.FromBnToTs(chain, uint64(apps[len(apps)-1].BlockNumber))

			s := simpleBounds{
				Count: uint64(count),
				FirstApp: types.RawAppearance{
					Address:          mon.Address.Hex(),
					BlockNumber:      apps[0].BlockNumber,
					TransactionIndex: apps[0].TransactionId,
				},
				FirstTs: firstTs,
				LastApp: types.RawAppearance{
					Address:          mon.Address.Hex(),
					BlockNumber:      apps[len(apps)-1].BlockNumber,
					TransactionIndex: apps[len(apps)-1].TransactionId,
				},
				LastTs: lastTs,
			}
			modelChan <- &s
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
