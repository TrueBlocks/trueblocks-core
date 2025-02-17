package tokensPkg

import (
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
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
			addr := base.HexToAddress(address) // if by_acct, this is token not address
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
					var tokenAddr base.Address
					if opts.ByAcct {
						tokenAddr = addr
						addr = singleAddr
					} else {
						tokenAddr = singleAddr
					}

					if balance, err := opts.Conn.GetBalanceAtToken(tokenAddr, addr, fmt.Sprintf("0x%x", bn)); balance == nil {
						errorChan <- err
					} else {
						if opts.Globals.Verbose {
							if bn == 0 || bn != currentBn {
								currentTs, _ = tslib.FromBnToTs(chain, bn)
							}
							currentBn = bn
						}
						s := &types.Token{
							Holder:      addr,
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

// TODO: NOTE THIS - DOES IT STILL WORK THIS WAY?
// } else if (by_acct) {
//     // All user-provided addresses are assumed to be tokens, except the last one which is the holder
//     holders.push_back(addrs[addrs.size() - 1]);
//     CAddressArray::iterator it;
//     it = prev(addrs.end());
//     addrs.erase(it);
//     for (auto addr : addrs) {
//         if (!isContractAt(addr, latestBlock))
//             errors.push_back("Address '" + addr + "' is not a token contract.");
//         else
//             tokens.push_back(addr);
//     }
