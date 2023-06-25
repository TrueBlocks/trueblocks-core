package tokensPkg

import (
	"context"
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/token"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *TokensOptions) HandleShow() error {
	chain := opts.Globals.Chain
	ctx, cancel := context.WithCancel(context.Background())
	tokenAddr := base.HexToAddress(opts.Addrs[0])
	fetchData := func(modelChan chan types.Modeler[types.RawTokenBal], errorChan chan error) {
		for index, address := range opts.Addrs {
			if index == 0 {
				continue
			}
			addr := base.HexToAddress(address)
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
					if bal, err := token.GetBalanceAt(chain, tokenAddr, addr, fmt.Sprintf("0x%x", bn)); bal == nil {
						errorChan <- err
					} else {
						s := &types.SimpleTokenBal{
							Holder:      addr,
							Address:     tokenAddr,
							Balance:     *bal,
							BlockNumber: 0x1036640,
							Decimals:    18,
							IsContract:  true,
							IsErc20:     true,
							Name:        "The Name",
							Symbol:      "The Symbol",
						}
						modelChan <- s
					}
				}
			}
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
