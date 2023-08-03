package tokensPkg

import (
	"context"
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/token"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *TokensOptions) HandleShow() error {
	if len(opts.Addrs) < 2 && len(opts.Parts) == 0 {
		logger.Fatal("Implementation error. Should not happen")
	}

	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	tokenAddr := base.HexToAddress(opts.Addrs[0])

	// TODO: Why does this have to dirty the caller?
	settings := rpcClient.DefaultRpcOptionsSettings{
		Chain: chain,
		Opts:  opts,
	}
	opts.Conn = settings.DefaultRpcOptions()

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawTokenBalance], errorChan chan error) {
		for _, address := range opts.Addrs[1:] {
			addr := base.HexToAddress(address)
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
					if bal, err := token.GetTokenBalanceAt(chain, tokenAddr, addr, fmt.Sprintf("0x%x", bn)); bal == nil {
						errorChan <- err
					} else {
						s := &types.SimpleTokenBalance{
							Holder:      addr,
							Address:     tokenAddr,
							Balance:     *bal,
							BlockNumber: bn,
							IsContract:  true,
							IsErc20:     true,
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
