package rpcClient

import (
	"errors"
	"math/big"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type GetStateField int

const (
	Balance GetStateField = 1 << iota
	Nonce
	Code
	Deployed
	Proxy
	Type
)

type GetStateFilters struct {
	Balance func(address base.Address, balance *big.Int) bool
}

// GetState returns account state
func (conn *Connection) GetState(fields GetStateField, address base.Address, blockNumber base.Blknum, filters GetStateFilters) (state *types.SimpleEthState, err error) {
	state = &types.SimpleEthState{
		Address:     address,
		BlockNumber: blockNumber,
		Deployed:    utils.NOPOS,
	}

	rpcPayload := []rpc.BatchPayload{
		{
			Key: "balance",
			Payload: &rpc.Payload{
				Method: "eth_getBalance",
				Params: rpc.Params{
					address,
					blockNumber,
				},
			},
		},
	}

	if (fields & Nonce) != 0 {
		rpcPayload = append(rpcPayload, rpc.BatchPayload{
			Key: "nonce",
			Payload: &rpc.Payload{
				Method: "eth_getTransactionCount",
				Params: rpc.Params{
					address,
					blockNumber,
				},
			},
		})
	}

	if (fields & Code) != 0 {
		rpcPayload = append(rpcPayload, rpc.BatchPayload{
			Key: "code",
			Payload: &rpc.Payload{
				Method: "eth_getCode",
				Params: rpc.Params{
					address,
					blockNumber,
				},
			},
		})
	}

	queryResults, err := rpc.QueryBatch[string](conn.Chain, rpcPayload)
	if err != nil {
		return
	}

	value := queryResults["balance"]
	balance := big.NewInt(0)
	balance.SetString(*value, 0)

	if filters.Balance != nil {
		if !filters.Balance(address, balance) {
			return nil, nil
		}
	}

	if (fields & Balance) != 0 {
		state.Balance = *balance
	}
	if value, ok := queryResults["nonce"]; ok && (fields&Nonce) != 0 {
		nonce, err := strconv.ParseUint(*value, 0, 64)
		if err != nil {
			return nil, err
		}
		state.Nonce = nonce
	}
	if value, ok := queryResults["code"]; ok && (fields&Code) != 0 {
		code := *value
		if code != "0x" {
			state.Code = code
		}
	}

	// deployedChan := make(chan struct{ block base.Blknum; err error })
	// if (mode & GetDeployed) != 0 {
	// 	go func ()  {
	// 		block, err := conn.GetContractDeployBlock(address)
	// 		deployedChan <- struct{block uint64; err error}{
	// 			block, err,
	// 		}
	// 		close(deployedChan)
	// 	}()
	// }

	if (fields & Deployed) != 0 {
		block, err := conn.GetContractDeployBlock(address)
		if err != nil && !errors.Is(err, ErrNotAContract) {
			return nil, err
		}
		// If err is ErrNotAContract, then we'll use the default value
		if err == nil {
			state.Deployed = block
		}
	}

	var proxy base.Address

	if (fields&Proxy) != 0 || (fields&Type) != 0 {
		proxy, err = conn.GetProxyAt(address, blockNumber)
		if err != nil {
			return
		}
		if (fields & Proxy) != 0 {
			state.Proxy = proxy
		}
	}

	if (fields & Type) != 0 {
		if !proxy.IsZero() {
			state.Accttype = "Proxy"
		} else {
			state.Accttype = conn.getTypeNonProxy(address, blockNumber)
		}
	}

	return
}

func (conn *Connection) getTypeNonProxy(address base.Address, blockNumber base.Blknum) string {
	isContractErr := conn.IsContractAt(address, &types.SimpleNamedBlock{BlockNumber: blockNumber})
	if errors.Is(isContractErr, ErrNotAContract) {
		return "EOA"
	}
	return "Contract"
}

func PartsToFields(parts []string, asEther bool) (stateFields GetStateField, outputFields []string, none bool) {
	balanceOutputField := "balance"
	if asEther {
		balanceOutputField = "ether"
	}

	if len(parts) == 0 {
		stateFields = Balance
		outputFields = []string{balanceOutputField}
		return
	}

	for _, part := range parts {
		switch part {
		case "none":
			none = true
			outputFields = nil
			return
		case "some":
			stateFields |= Balance | Nonce | Code | Type
		case "all":
			stateFields |= Balance | Nonce | Code | Proxy | Deployed | Type
		case "balance":
			stateFields |= Balance
		case "nonce":
			stateFields |= Nonce
		case "code":
			stateFields |= Code
		case "proxy":
			stateFields |= Proxy
		case "deployed":
			stateFields |= Deployed
		case "accttype":
			stateFields |= Type
		}
	}

	outputFields = make([]string, 0, 6)
	if (stateFields & Proxy) != 0 {
		outputFields = append(outputFields, "proxy")
	}

	// Always show balance for non-none parts
	stateFields |= Balance
	outputFields = append(outputFields, balanceOutputField)

	if (stateFields & Nonce) != 0 {
		outputFields = append(outputFields, "nonce")
	}
	if (stateFields & Code) != 0 {
		outputFields = append(outputFields, "code")
	}
	if (stateFields & Deployed) != 0 {
		outputFields = append(outputFields, "deployed")
	}
	if (stateFields & Type) != 0 {
		outputFields = append(outputFields, "accttype")
	}

	return
}
