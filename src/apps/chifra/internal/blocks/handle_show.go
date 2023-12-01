// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

import (
	"context"
	"errors"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum"
)

func (opts *BlocksOptions) HandleShow() error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	nErrors := 0

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawBlock], errorChan chan error) {
		if sliceOfMaps, cnt, err := identifiers.AsSliceOfMaps[types.SimpleBlock[types.SimpleTransaction]](chain, opts.BlockIds); err != nil {
			errorChan <- err
			cancel()

		} else if cnt == 0 {
			errorChan <- fmt.Errorf("no blocks found for the query")
			cancel()

		} else {
			bar := logger.NewBar(logger.BarOptions{
				Enabled: !testMode && !utils.IsTerminal(),
				Total:   int64(cnt),
			})

			for _, thisMap := range sliceOfMaps {
				thisMap := thisMap
				for app := range thisMap {
					thisMap[app] = new(types.SimpleBlock[types.SimpleTransaction])
				}

				iterFunc := func(app types.SimpleAppearance, value *types.SimpleBlock[types.SimpleTransaction]) error {
					bn := uint64(app.BlockNumber)
					if block, err := opts.Conn.GetBlockBodyByNumber(bn); err != nil {
						errorChan <- err
						if errors.Is(err, ethereum.NotFound) {
							errorChan <- errors.New("uncles not found")
						}
						cancel()
						return nil
					} else {
						bar.Tick()
						*value = block
					}
					return nil
				}

				iterErrorChan := make(chan error)
				iterCtx, iterCancel := context.WithCancel(context.Background())
				defer iterCancel()
				go utils.IterateOverMap(iterCtx, iterErrorChan, thisMap, iterFunc)
				for err := range iterErrorChan {
					if !testMode || nErrors == 0 {
						errorChan <- err
						nErrors++
					}
				}

				items := make([]*types.SimpleBlock[types.SimpleTransaction], 0, len(thisMap))
				for _, item := range thisMap {
					items = append(items, item)
				}
				sort.Slice(items, func(i, j int) bool {
					if items[i].BlockNumber == items[j].BlockNumber {
						return items[i].Hash.Hex() < items[j].Hash.Hex()
					}
					return items[i].BlockNumber < items[j].BlockNumber
				})

				for _, item := range items {
					item := item
					modelChan <- item
				}
			}
			bar.Finish(true)
		}
	}

	extra := map[string]interface{}{
		"hashes":     opts.Hashes,
		"uncles":     opts.Uncles,
		"articulate": opts.Articulate,
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
