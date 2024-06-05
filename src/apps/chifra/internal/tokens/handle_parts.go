package tokensPkg

import (
	"context"
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *TokensOptions) HandleParts() error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		for _, address := range opts.Addrs {
			addr := base.HexToAddress(address)
			currentBn := base.Blknum(0)
			currentTs := base.Timestamp(0)
			for _, br := range opts.BlockIds {
				blockNums, err := br.ResolveBlocks(chain)
				if err != nil {
					errorChan <- err
					if errors.Is(err, ethereum.NotFound) {
						continue
					}
					cancel()
					return
				}

				for _, bn := range blockNums {
					if state, err := opts.Conn.GetTokenState(addr, fmt.Sprintf("0x%x", bn)); err != nil {
						errorChan <- err
					} else {
						s := &types.Token{
							Address:     state.Address,
							BlockNumber: bn,
							TotalSupply: state.TotalSupply,
							Decimals:    uint64(state.Decimals),
						}
						if opts.Globals.Verbose {
							if bn == 0 || bn != currentBn {
								currentTs, _ = tslib.FromBnToTs(chain, bn)
							}
							s.Timestamp = currentTs
							currentBn = bn
						}
						modelChan <- s
					}
				}
			}
		}
	}

	nameTypes := names.Custom | names.Prefund | names.Regular
	namesMap, err := names.LoadNamesMap(chain, nameTypes, nil)
	if err != nil {
		return err
	}

	extraOpts := map[string]any{
		"testMode": testMode,
		"namesMap": namesMap,
		"parts":    opts.Parts,
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
