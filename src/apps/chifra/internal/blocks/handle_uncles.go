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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum"
)

func (opts *BlocksOptions) HandleUncles() error {
	chain := opts.Globals.Chain
	nErrors := 0

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawBlock], errorChan chan error) {
		var err error
		var appMap map[identifiers.ResolvedId]*types.SimpleBlock[types.SimpleTransaction]
		if appMap, _, err = identifiers.AsMap[types.SimpleBlock[types.SimpleTransaction]](chain, opts.BlockIds); err != nil {
			errorChan <- err
			cancel()
		}

		iterCtx, iterCancel := context.WithCancel(context.Background())
		defer iterCancel()

		uncles := make([]types.SimpleBlock[types.SimpleTransaction], 0, len(appMap))
		bar := logger.NewBar(logger.BarOptions{
			Type:    logger.Expanding,
			Enabled: !opts.Globals.TestMode && len(opts.Globals.File) == 0,
			Total:   int64(len(appMap)),
		})

		iterFunc := func(app identifiers.ResolvedId, value *types.SimpleBlock[types.SimpleTransaction]) error {
			if uncs, err := opts.Conn.GetUncleBodiesByNumber(app.BlockNumber); err != nil {
				errorChan <- err
				if errors.Is(err, ethereum.NotFound) {
					errorChan <- errors.New("uncles not found")
				}
				cancel()
				return nil
			} else {
				for _, uncle := range uncs {
					uncle := uncle
					bar.Tick()
					if uncle.BlockNumber > 0 {
						uncles = append(uncles, uncle)
					}
				}
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
		bar.Finish(true)

		sort.Slice(uncles, func(i, j int) bool {
			if uncles[i].BlockNumber == uncles[j].BlockNumber {
				return uncles[i].Hash.Hex() < uncles[j].Hash.Hex()
			}
			return uncles[i].BlockNumber < uncles[j].BlockNumber
		})

		for _, item := range uncles {
			item := item
			modelChan <- &item
		}
	}

	extra := map[string]interface{}{
		"hashes":     opts.Hashes,
		"count":      opts.Count,
		"uncles":     opts.Uncles,
		"articulate": opts.Articulate,
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
