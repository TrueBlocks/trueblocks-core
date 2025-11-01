package tokensPkg

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *TokensOptions) HandleShow(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain

	var singleAddr base.Address
	var addrRange []string

	if opts.ByAcct {
		singleAddr = base.HexToAddress(opts.Addrs[len(opts.Addrs)-1]) // by_acct case. Last address is for balance, all others are token addresses.
		addrRange = opts.Addrs[:len(opts.Addrs)-1]
	} else {
		singleAddr = base.HexToAddress(opts.Addrs[0]) // normal case. First address is token, the rest are addresses for balances.
		addrRange = opts.Addrs[1:]
	}

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		for _, address := range addrRange {
			currentBn := base.Blknum(0)
			currentTs := base.Timestamp(0)
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

				for _, bn := range blockNums {
					tokenAddr := singleAddr
					holder := base.HexToAddress(address)
					if opts.ByAcct {
						tokenAddr = base.HexToAddress(address)
						holder = singleAddr
					}

					if balance, err := opts.Conn.GetBalanceAtToken(tokenAddr, holder, bn); balance == nil {
						errorChan <- err
					} else {
						if opts.Globals.Verbose {
							if bn == 0 || bn != currentBn {
								currentTs, _ = tslib.FromBnToTs(chain, bn)
							}
							currentBn = bn
						}
						s := &types.Token{
							Holder:      holder,
							Address:     tokenAddr,
							Balance:     *balance,
							BlockNumber: bn,
							Timestamp:   currentTs,
							TokenType:   types.TokenErc20,
						}
						modelChan <- s
					}
				}
			}
		}
	}

	extraOpts := map[string]any{
		"parts":     []string{"all_held"},
		"loadNames": true,
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
