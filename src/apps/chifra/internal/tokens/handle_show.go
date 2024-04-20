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

func (opts *TokensOptions) HandleShow() error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	tokenAddr := base.HexToAddress(opts.Addrs[0])

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawToken], errorChan chan error) {
		for _, address := range opts.Addrs[1:] {
			addr := base.HexToAddress(address)
			currentBn := uint64(0)
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
					if bal, err := opts.Conn.GetBalanceAtToken(tokenAddr, addr, fmt.Sprintf("0x%x", bn)); bal == nil {
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
							Balance:     *bal,
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

	nameParts := names.Custom | names.Prefund | names.Regular
	namesMap, err := names.LoadNamesMap(chain, nameParts, nil)
	if err != nil {
		return err
	}

	extra := map[string]interface{}{
		"testMode": testMode,
		"namesMap": namesMap,
		"parts":    []string{"all_held"},
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
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
