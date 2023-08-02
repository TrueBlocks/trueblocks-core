package tokensPkg

import (
	"context"
	"errors"
	"fmt"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/token"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *TokensOptions) HandleParts() error {
	if len(opts.Parts) == 0 {
		logger.Fatal("Implementation error. Should not happen")
	}

	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	// TODO: Why does this have to dirty the caller?
	settings := rpcClient.DefaultRpcOptionsSettings{
		Chain: chain,
		Opts:  opts,
	}
	opts.Conn = settings.DefaultRpcOptions()
	if !opts.Conn.Store.ReadOnly() {
		opts.Conn.LatestBlockTimestamp = opts.Conn.GetBlockTimestamp(chain, nil)
	}

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawTokenBalance], errorChan chan error) {
		for _, address := range opts.Addrs {
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
					if state, err := token.GetState(chain, addr, fmt.Sprintf("0x%x", bn)); err != nil {
						errorChan <- err
					} else {
						ts, _ := new(big.Int).SetString(state.TotalSupply, 10)
						s := &types.SimpleTokenBalance{
							Address:     state.Address,
							BlockNumber: bn,
							TotalSupply: *ts,
							Decimals:    uint64(state.Decimals),
						}
						modelChan <- s
					}
				}
			}
		}
	}

	parts := names.Custom | names.Prefund | names.Regular
	namesMap, err := names.LoadNamesMap(chain, parts, nil)
	if err != nil {
		return err
	}

	extra := map[string]interface{}{
		"testMode": testMode,
		"namesMap": namesMap,
		"parts":    opts.Parts,
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

// func (opts *TokensOptions) PartsToFields() (stateFields account.GetStateField, outputFields []string, none bool) {
// 	balanceOutputField := "balance"
// 	if opts.Globals.Ether {
// 		balanceOutputField = "ether"
// 	}

// 	if len(opts.Parts) == 0 {
// 		stateFields = account.Balance
// 		outputFields = []string{balanceOutputField}
// 		return
// 	}

// 	for _, part := range opts.Parts {
// 		switch part {
// 		case "none":
// 			none = true
// 			outputFields = nil
// 			return
// 		case "some":
// 			stateFields |= account.Balance | account.Nonce | account.Code | account.Type
// 		case "all":
// 			stateFields |= account.Balance | account.Nonce | account.Code | account.Proxy | account.Deployed | account.Type
// 		case "balance":
// 			stateFields |= account.Balance
// 		case "nonce":
// 			stateFields |= account.Nonce
// 		case "code":
// 			stateFields |= account.Code
// 		case "proxy":
// 			stateFields |= account.Proxy
// 		case "deployed":
// 			stateFields |= account.Deployed
// 		case "accttype":
// 			stateFields |= account.Type
// 		}
// 	}

// 	outputFields = make([]string, 0, 6)
// 	if (stateFields & account.Proxy) != 0 {
// 		outputFields = append(outputFields, "proxy")
// 	}

// 	// Always show balance for non-none parts
// 	stateFields |= account.Balance
// 	outputFields = append(outputFields, balanceOutputField)

// 	if (stateFields & account.Nonce) != 0 {
// 		outputFields = append(outputFields, "nonce")
// 	}
// 	if (stateFields & account.Code) != 0 {
// 		outputFields = append(outputFields, "code")
// 	}
// 	if (stateFields & account.Deployed) != 0 {
// 		outputFields = append(outputFields, "deployed")
// 	}
// 	if (stateFields & account.Type) != 0 {
// 		outputFields = append(outputFields, "accttype")
// 	}

// 	return
// }
