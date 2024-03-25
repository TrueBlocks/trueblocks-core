package rpc

import (
	"context"
	"errors"
	"fmt"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// StatePart is a bit mask for querying various parts an address's state
type StatePart int

const (
	// The balance of the address
	Balance StatePart = 1 << iota
	// The nonce of the address
	Nonce
	// The code of the address if the address is a smart contract
	Code
	// The block number when the smart contract was deployed
	Deployed
	// If an address is proxied, the address of the proxy (note that the proxy may in some cases not be visible and therefore this value will be empty)
	Proxy
	// Either EOA or Contract
	Type
)

type StateFilters struct {
	Balance func(address base.Address, balance *base.Wei) bool
}

// GetState returns account state
func (conn *Connection) GetState(fieldBits StatePart, address base.Address, blockNumber base.Blknum, filters StateFilters) (state *types.SimpleState, err error) {
	state = &types.SimpleState{
		Address:     address,
		BlockNumber: blockNumber,
		Deployed:    utils.NOPOS,
	}

	rpcPayload := []query.BatchPayload{
		{
			Key: "balance",
			Payload: &query.Payload{
				Method: "eth_getBalance",
				Params: query.Params{
					address,
					fmt.Sprintf("0x%x", blockNumber),
				},
			},
		},
	}

	if (fieldBits & Nonce) != 0 {
		rpcPayload = append(rpcPayload, query.BatchPayload{
			Key: "nonce",
			Payload: &query.Payload{
				Method: "eth_getTransactionCount",
				Params: query.Params{
					address,
					fmt.Sprintf("0x%x", blockNumber),
				},
			},
		})
	}

	if (fieldBits & Code) != 0 {
		rpcPayload = append(rpcPayload, query.BatchPayload{
			Key: "code",
			Payload: &query.Payload{
				Method: "eth_getCode",
				Params: query.Params{
					address,
					fmt.Sprintf("0x%x", blockNumber),
				},
			},
		})
	}

	queryResults, err := query.QueryBatch[string](conn.Chain, rpcPayload)
	if err != nil {
		return
	}

	value := queryResults["balance"]
	balance := base.NewWei(0)
	balance.SetString(*value, 0)

	if filters.Balance != nil {
		if !filters.Balance(address, balance) {
			return nil, nil
		}
	}

	if (fieldBits & Balance) != 0 {
		state.Balance = *balance
	}
	if value, ok := queryResults["nonce"]; ok && (fieldBits&Nonce) != 0 {
		nonce, err := strconv.ParseUint(*value, 0, 64)
		if err != nil {
			return nil, err
		}
		state.Nonce = nonce
	}
	if value, ok := queryResults["code"]; ok && (fieldBits&Code) != 0 {
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

	if (fieldBits & Deployed) != 0 {
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

	if (fieldBits&Proxy) != 0 || (fieldBits&Type) != 0 {
		proxy, err = conn.GetContractProxyAt(address, blockNumber)
		if err != nil {
			return
		}
		if (fieldBits & Proxy) != 0 {
			state.Proxy = proxy
		}
	}

	if (fieldBits & Type) != 0 {
		if !proxy.IsZero() {
			state.AccountType = "Proxy"
		} else {
			state.AccountType = conn.getTypeNonProxy(address, blockNumber)
		}
	}

	return
}

// GetBalanceAt returns a balance for an address at a block
func (conn *Connection) GetBalanceAt(addr base.Address, bn uint64) (*base.Wei, error) {
	if ec, err := conn.getClient(); err != nil {
		var zero base.Wei
		return &zero, err
	} else {
		defer ec.Close()
		ret, err := ec.BalanceAt(context.Background(), addr.Common(), base.BiFromUint64(bn))
		return (*base.Wei)(ret), err
	}
}

// GetFieldsFromParts converts a string array of part names to a bit mask of parts and returns the corresponding output field names or none if no valid parts are present
func (conn *Connection) GetFieldsFromParts(parts []string, asEther bool) (stateFields StatePart, outputFields []string, none bool) {
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

func (conn *Connection) getTypeNonProxy(address base.Address, bn base.Blknum) string {
	isContractErr := conn.IsContractAt(address, bn)
	if errors.Is(isContractErr, ErrNotAContract) {
		return "EOA"
	}
	return "Contract"
}
