// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
	"github.com/ethereum/go-ethereum/common"
)

func (opts *WhenOptions) HandleShowBlocks() error {

	ctx, cancel := context.WithCancel(context.Background())

	// Note: Make sure to add an entry to enabledForCmd in src/apps/chifra/pkg/output/helpers.go
	fetchData := func(modelChan chan types.Modeler[types.RawNamedBlock], errorChan chan error) {
		for _, br := range opts.BlockIds {
			blockNums, err := br.ResolveBlocks(opts.Globals.Chain)
			if err != nil {
				errorChan <- err
				if errors.Is(err, ethereum.NotFound) {
					continue
				}
				cancel()
				return
			}

			for _, bn := range blockNums {
				block, err := rpcClient.GetBlockHeaderByNumber(opts.Globals.Chain, bn)
				if err != nil {
					errorChan <- err
					if errors.Is(err, ethereum.NotFound) {
						continue
					}
					cancel()
					return
				}
				if br.StartType == identifiers.BlockHash && common.HexToHash(br.Orig) != block.Hash {
					errorChan <- errors.New("block hash not found")
					continue
				}

				d, _ := tslib.FromTsToDate(block.GetTimestamp())
				nm, _ := tslib.FromBnToName(opts.Globals.Chain, block.BlockNumber)
				modelChan <- &types.SimpleNamedBlock{
					BlockNumber: block.BlockNumber,
					Timestamp:   int64(block.GetTimestamp()),
					Date:        d.Format("YYYY-MM-DD HH:mm:ss UTC"),
					Name:        nm,
				}
			}
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
	})
}
