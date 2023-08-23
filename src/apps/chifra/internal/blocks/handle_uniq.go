// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

import (
	"context"
	"errors"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/uniq"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum"
)

func (opts *BlocksOptions) HandleUniq() error {
	chain := opts.Globals.Chain
	nErrors := 0

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawAppearance], errorChan chan error) {
		var err error
		var appMap map[identifiers.ResolvedId]*types.SimpleAppearance
		if appMap, _, err = identifiers.AsMap[types.SimpleAppearance](chain, opts.BlockIds); err != nil {
			errorChan <- err
			cancel()
		}

		iterCtx, iterCancel := context.WithCancel(context.Background())
		defer iterCancel()

		apps := make([]types.SimpleAppearance, 0, len(appMap))
		bar := logger.NewBar(logger.BarOptions{
			Type:    logger.Expanding,
			Enabled: !opts.Globals.TestMode && len(opts.Globals.File) == 0,
			Total:   int64(len(appMap)),
		})
		iterFunc := func(app identifiers.ResolvedId, value *types.SimpleAppearance) error {
			procFunc := func(s *types.SimpleAppearance) error {
				bar.Tick()
				apps = append(apps, *s)
				return nil
			}

			if err := uniq.GetUniqAddressesInBlock(chain, opts.Flow, opts.Conn, procFunc, app.BlockNumber); err != nil {
				errorChan <- err
				if errors.Is(err, ethereum.NotFound) {
					return nil
				}
				cancel()
			}
			return nil
		}

		iterErrorChan := make(chan error)
		go utils.IterateOverMap(iterCtx, iterErrorChan, appMap, iterFunc)
		for err := range iterErrorChan {
			if !opts.Globals.TestMode || nErrors == 0 {
				errorChan <- err
				nErrors++
			}
		}
		bar.Finish(true /* newLine */)

		items := make([]types.SimpleAppearance, 0, len(appMap))
		for _, app := range apps {
			app := app
			items = append(items, app)
		}
		sort.Slice(items, func(i, j int) bool {
			if items[i].BlockNumber == items[j].BlockNumber {
				if items[i].TransactionIndex == items[j].TransactionIndex {
					return items[i].Reason < items[j].Reason
				}
				return items[i].TransactionIndex < items[j].TransactionIndex
			}
			return items[i].BlockNumber < items[j].BlockNumber
		})

		for _, s := range items {
			s := s
			modelChan <- &s
		}
	}

	extra := map[string]interface{}{
		"uniq": true,
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
