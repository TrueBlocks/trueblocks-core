// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *WhenOptions) HandleShow(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		for _, br := range opts.BlockIds {
			blockNums, err := br.ResolveBlocks(chain)
			if err != nil {
				errorChan <- err
				if errors.Is(err, ethereum.NotFound) {
					continue
				}
				rCtx.Cancel()
				return
			}

			showProgress := opts.Globals.ShowProgress()
			bar := logger.NewBar(logger.BarOptions{
				Enabled: showProgress,
				Total:   int64(len(blockNums)),
			})

			for _, bn := range blockNums {
				block, err := opts.Conn.GetBlockHeaderByNumber(bn)
				if err != nil {
					errorChan <- err
					if errors.Is(err, ethereum.NotFound) {
						continue
					}
					rCtx.Cancel()
					return
				}
				if br.StartType == identifiers.BlockHash && base.HexToHash(br.Orig) != block.Hash {
					errorChan <- errors.New("block hash not found")
					continue
				}

				nb, _ := tslib.FromBnToNamedBlock(chain, block.BlockNumber)
				if nb == nil {
					modelChan <- &types.NamedBlock{
						BlockNumber: block.BlockNumber,
						Timestamp:   block.Timestamp,
					}
				} else {
					modelChan <- nb
				}
				bar.Tick()
			}
			bar.Finish(true)
		}
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
