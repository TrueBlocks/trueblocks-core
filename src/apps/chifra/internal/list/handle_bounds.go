package listPkg

import (
	"context"
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
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

type simpleBounds struct {
	Count    uint64              `json:"count"`
	FirstApp types.RawAppearance `json:"firstApp"`
	FirstTs  base.Timestamp      `json:"firstTs"`
	LastApp  types.RawAppearance `json:"lastApp"`
	LastTs   base.Timestamp      `json:"lastTs"`
}

func (s *simpleBounds) Raw() *types.RawModeler {
	return nil
}

func (s *simpleBounds) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	return types.Model{
		Data: map[string]interface{}{
			"address":   s.FirstApp.Address,
			"count":     s.Count,
			"firstApp":  fmt.Sprintf("%d.%d", s.FirstApp.BlockNumber, s.FirstApp.TransactionIndex),
			"firstTs":   s.FirstTs,
			"firstDate": utils.FormattedDate(s.FirstTs),
			"lastApp":   fmt.Sprintf("%d.%d", s.LastApp.BlockNumber, s.LastApp.TransactionIndex),
			"lastTs":    s.LastTs,
			"lastDate":  utils.FormattedDate(s.LastTs),
			"blockSpan": (s.LastApp.BlockNumber - s.FirstApp.BlockNumber),
			"blockFreq": uint64(s.LastApp.BlockNumber-s.FirstApp.BlockNumber) / s.Count,
		},
		Order: []string{
			"address",
			"count",
			"firstApp",
			"firstTs",
			"firstDate",
			"lastApp",
			"lastTs",
			"lastDate",
			"blockSpan",
			"blockFreq",
		},
	}
}
