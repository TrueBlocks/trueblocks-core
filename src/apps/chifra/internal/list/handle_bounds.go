package listPkg

import (
	"context"
	"errors"
	"fmt"
	"io"

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

	return output.StreamMany(ctx, fetchData, output.OutputOptions{
		Writer:     opts.Globals.Writer,
		Chain:      opts.Globals.Chain,
		TestMode:   opts.Globals.TestMode,
		NoHeader:   opts.Globals.NoHeader,
		ShowRaw:    opts.Globals.ShowRaw,
		Verbose:    opts.Globals.Verbose,
		LogLevel:   opts.Globals.LogLevel,
		Format:     opts.Globals.Format,
		OutputFn:   opts.Globals.OutputFn,
		Append:     opts.Globals.Append,
		JsonIndent: "  ",
		Extra: map[string]interface{}{
			"verbose": opts.Globals.Verbose,
		},
	})
}

type RawGeneric struct {
}
type SimpleGeneric struct {
	First types.RawAppearance
	Last  types.RawAppearance
	raw   *RawGeneric
}

func (s *SimpleGeneric) Raw() *RawGeneric {
	return s.raw
}

func (s *SimpleGeneric) SetRaw(raw *RawGeneric) {
	s.raw = raw
}

func (s *SimpleGeneric) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"address": s.First.Address,
		"first":   fmt.Sprintf("%d.%d", s.First.BlockNumber, s.First.TransactionIndex),
		"last":    fmt.Sprintf("%d.%d", s.Last.BlockNumber, s.Last.TransactionIndex),
		"range":   (s.Last.BlockNumber - s.First.BlockNumber),
	}
	order = []string{
		"address",
		"first",
		"last",
		"range",
	}
	// EXISTING_CODE

	return types.Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleGeneric) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleGeneric) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}
