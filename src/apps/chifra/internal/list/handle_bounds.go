package listPkg

import (
	"context"
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ListOptions) HandleBounds(monitorArray []monitor.Monitor) error {
	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[RawGeneric], errorChan chan error) {
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

			s := SimpleGeneric{
				First: types.RawAppearance{
					Address:          mon.Address.Hex(),
					BlockNumber:      apps[0].BlockNumber,
					TransactionIndex: apps[0].TransactionId,
				},
				Last: types.RawAppearance{
					Address:          mon.Address.Hex(),
					BlockNumber:      apps[len(apps)-1].BlockNumber,
					TransactionIndex: apps[len(apps)-1].TransactionId,
				},
			}
			modelChan <- &s
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

type RawGeneric interface{}

type SimpleGeneric struct {
	First types.RawAppearance
	Last  types.RawAppearance
	raw   *RawGeneric
}

func (s *SimpleGeneric) Raw() *RawGeneric {
	return s.raw
}

func (s *SimpleGeneric) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	return types.Model{
		Data: map[string]interface{}{
			"address": s.First.Address,
			"first":   fmt.Sprintf("%d.%d", s.First.BlockNumber, s.First.TransactionIndex),
			"last":    fmt.Sprintf("%d.%d", s.Last.BlockNumber, s.Last.TransactionIndex),
			"range":   (s.Last.BlockNumber - s.First.BlockNumber),
		},
		Order: []string{
			"address",
			"first",
			"last",
			"range",
		},
	}
}
