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

func (opts *StateOptions) HandleShow() error {
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
					return len(balance.Bytes()) > 0
				}
				return true
			},
		}
	}

	stateFields, outputFields, none := opts.PartsToFields()

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawEthState], errorChan chan error) {
		for _, addressStr := range opts.Addrs {
			address := base.HexToAddress(addressStr)
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
					if none {
						modelChan <- &types.SimpleEthState{
							Address:     address,
							BlockNumber: bn,
						}
						return
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
	balanceOutputField := "balance"
	if opts.Globals.Ether {
		balanceOutputField = "ether"
	}

	if len(opts.Parts) == 0 {
		stateFields = account.Balance
		outputFields = []string{balanceOutputField}
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
		case "all":
			stateFields |= account.Balance | account.Nonce | account.Code | account.Proxy | account.Deployed | account.Type
		case "balance":
			stateFields |= account.Balance
		case "nonce":
			stateFields |= account.Nonce
		case "code":
			stateFields |= account.Code
		case "proxy":
			stateFields |= account.Proxy
		case "deployed":
			stateFields |= account.Deployed
		case "accttype":
			stateFields |= account.Type
		}
	}

	outputFields = make([]string, 0, 6)
	if (stateFields & account.Proxy) != 0 {
		outputFields = append(outputFields, "proxy")
	}

	// Always show balance for non-none parts
	stateFields |= account.Balance
	outputFields = append(outputFields, balanceOutputField)

	if (stateFields & account.Nonce) != 0 {
		outputFields = append(outputFields, "nonce")
	}
	if (stateFields & account.Code) != 0 {
		outputFields = append(outputFields, "code")
	}
	if (stateFields & account.Deployed) != 0 {
		outputFields = append(outputFields, "deployed")
	}
	if (stateFields & account.Type) != 0 {
		outputFields = append(outputFields, "accttype")
	}

	return
}
