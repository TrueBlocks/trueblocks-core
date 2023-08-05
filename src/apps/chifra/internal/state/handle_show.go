package statePkg

import (
	"context"
	"errors"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *StateOptions) HandleShow() error {
	chain := opts.Globals.Chain
	// TODO: Why does this have to dirty the caller?
	settings := rpcClient.DefaultRpcOptionsSettings{
		Chain: chain,
		Opts:  opts,
	}
	opts.Conn = settings.DefaultRpcOptions()

	previousBalance := make(map[base.Address]*big.Int, len(opts.Addrs))
	var filters rpcClient.StateFilters
	if opts.Changes || opts.NoZero {
		filters = rpcClient.StateFilters{
			Balance: func(address base.Address, balance *big.Int) bool {
				if opts.Changes {
					previous := previousBalance[address]
					if balance == previous {
						return false
					}
					previousBalance[address] = balance
				}

				if opts.NoZero {
					return len(balance.Bytes()) > 0
				}

				return true
			},
		}
	}

	stateFields, outputFields, none := rpcClient.PartsToFields(opts.Parts, opts.Globals.Ether)

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawEthState], errorChan chan error) {
		for _, addressStr := range opts.Addrs {
			address := base.HexToAddress(addressStr)
			for _, br := range opts.BlockIds { // TODO: use the regular way to do this
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
					if none {
						modelChan <- &types.SimpleEthState{
							Address:     address,
							BlockNumber: bn,
						}
						return
					}

					state, err := opts.Conn.GetState(
						stateFields,
						address,
						bn,
						filters,
					)
					if err != nil {
						errorChan <- err
					}
					// state may be nil if it was skipped by a filter for example
					if state != nil {
						modelChan <- state
					}
				}
			}
		}
	}

	extra := map[string]interface{}{
		"fields": outputFields,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
