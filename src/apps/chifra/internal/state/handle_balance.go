package statePkg

import (
	"context"
	"errors"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/account"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *StateOptions) HandleBalance() error {
	chain := opts.Globals.Chain
	previousBalance := make(map[base.Address]*big.Int, len(opts.Addrs))
	var filters account.GetStateFilters
	if opts.Changes || opts.NoZero {
		filters = account.GetStateFilters{
			Balance: func(address base.Address, balance *big.Int) bool {
				if opts.Changes {
					previous := previousBalance[address]
					if balance == previous {
						return false
					}
					previousBalance[address] = balance
				}

				if opts.NoZero {
					return balance != big.NewInt(0)
				}
				return true
			},
		}
	}

	stateFields, outputFields, none := opts.PartsToFields()

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawEthState], errorChan chan error) {
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
				for _, addressStr := range opts.Addrs {
					address := base.HexToAddress(addressStr)
					if none {
						modelChan <- &types.SimpleEthState{
							Address:     address,
							BlockNumber: bn,
						}
					}

					state, err := account.GetState(
						chain,
						stateFields,
						address,
						bn,
						filters,
					)
					if err != nil {
						errorChan <- err
					}
					// `state` can be nil if it was skipped by a filter
					if state != nil {
						modelChan <- state
					}
				}
			}
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(map[string]interface{}{
		"fields": outputFields,
	}))
}

func (opts *StateOptions) PartsToFields() (stateFields account.GetStateField, outputFields []string, none bool) {
	outputFields = make([]string, 0)
	if len(opts.Parts) == 0 {
		stateFields = account.Balance
		outputFields = []string{"balance"}
		return
	}

	for _, part := range opts.Parts {
		switch part {
		case "none":
			none = true
			outputFields = nil
			return
		case "some":
			stateFields |= account.Balance | account.Nonce | account.Code | account.Type
			outputFields = append(outputFields, []string{
				"balance",
				"nonce",
				"code",
				"type",
			}...)
		case "all":
			stateFields |= account.Balance | account.Nonce | account.Code | account.Proxy | account.Deployed | account.Type
			outputFields = []string{
				"balance",
				"nonce",
				"code",
				"proxy",
				"deployed",
				"type",
			}
			return
		case "balance":
			stateFields |= account.Balance
			outputFields = append(outputFields, "balance")
		case "nonce":
			stateFields |= account.Nonce
			outputFields = append(outputFields, "nonce")
		case "code":
			stateFields |= account.Code
			outputFields = append(outputFields, "code")
		case "proxy":
			stateFields |= account.Proxy
			outputFields = append(outputFields, "proxy")
		case "deployed":
			stateFields |= account.Deployed
			outputFields = append(outputFields, "deployed")
		case "accttype":
			stateFields |= account.Type
			outputFields = append(outputFields, "type")
		}
	}
	return
}
