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

func (opts *BlocksOptions) HandleUncles() error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	nErrors := 0

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawBlock], errorChan chan error) {
		if sliceOfMaps, cnt, err := identifiers.SliceOfMaps_AsMaps[types.SimpleBlock[string]](chain, opts.BlockIds); err != nil {
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
					thisMap[app] = new(types.SimpleBlock[string])
				}

				uncles := make([]types.SimpleBlock[types.SimpleTransaction], 0, len(thisMap))
				iterFunc := func(app types.SimpleAppearance, value *types.SimpleBlock[string]) error {
					bn := uint64(app.BlockNumber)
					if uncs, err := opts.Conn.GetUncleBodiesByNumber(bn); err != nil {
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
				iterCtx, iterCancel := context.WithCancel(context.Background())
				defer iterCancel()
				go utils.IterateOverMap(iterCtx, iterErrorChan, thisMap, iterFunc)
				for err := range iterErrorChan {
					if !testMode || nErrors == 0 {
						errorChan <- err
						nErrors++
					}
				}

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
