package rpc

import (
	"context"
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

type StateFilters struct {
	BalanceCheck func(address base.Address, balance *base.Wei) bool
}

// GetState returns account state (search: FromRpc)
func (conn *Connection) GetState(fieldBits types.StatePart, address base.Address, blockNumber base.Blknum, filters StateFilters) (*types.State, error) {
	state := &types.State{
		BlockNumber: blockNumber,
		Address:     address,
	}
	if err := conn.ReadFromCache(state); err == nil {
		if state.Parts&fieldBits == fieldBits {
			return state, nil
		}
		fieldBits |= state.Parts // preserve what's there
	}

	// We always ask for balance even if we dont' need it. Not sure why.
	blockTs := conn.GetBlockTimestamp(blockNumber)
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

	if (fieldBits & types.Nonce) != 0 {
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

	if (fieldBits & types.Code) != 0 {
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
		return nil, err
	}

	value := queryResults["balance"]
	balance := base.NewWei(0)
	balance.SetString(*value, 0)

	state = &types.State{
		Address:     address,
		BlockNumber: blockNumber,
		Deployed:    base.NOPOSN,
		Timestamp:   blockTs,
		Parts:       fieldBits,
	}

	if (fieldBits & types.Balance) != 0 {
		state.Balance = *balance
	}

	if (fieldBits & types.Nonce) != 0 {
		if value, ok := queryResults["nonce"]; ok {
			state.Nonce = base.MustParseValue(*value)
		}
	}

	if (fieldBits & types.Code) != 0 {
		if value, ok := queryResults["code"]; ok {
			code := *value
			if code != "0x" {
				state.Code = code
			}
		}
	}

	if (fieldBits & types.Deployed) != 0 {
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

	if (fieldBits&types.Proxy) != 0 || (fieldBits&types.Type) != 0 {
		proxy, err = conn.GetContractProxyAt(address, blockNumber)
		if err != nil {
			return nil, err
		}
		if (fieldBits & types.Proxy) != 0 {
			state.Proxy = proxy
		}
	}

	if (fieldBits & types.Type) != 0 {
		if !proxy.IsZero() {
			state.AccountType = "Proxy"
		} else {
			state.AccountType = conn.getTypeNonProxy(address, blockNumber)
		}
	}

	err = conn.WriteToCache(state, walk.Cache_State, blockTs)
	if filters.BalanceCheck != nil {
		if !filters.BalanceCheck(address, &state.Balance) {
			return nil, err
		}
	}
	return state, err
}

// GetBalanceAt returns a balance for an address at a block
func (conn *Connection) GetBalanceAt(addr base.Address, bn base.Blknum) (*base.Wei, error) {
	var balance *base.Wei

	// TODO: BOGUS - THIS IN MEMORY CACHE IS GOOD, BUT COULD BE BINARY FILE
	key := fmt.Sprintf("%s|%d", addr.Hex(), bn)
	conn.cacheMutex.Lock()
	var ok bool
	if balance, ok = conn.balanceCache[key]; ok {
		conn.cacheMutex.Unlock()
		return balance, nil
	}
	conn.cacheMutex.Unlock()

	if ec, err := conn.getClient(); err != nil {
		var zero base.Wei
		return &zero, err
	} else {
		defer ec.Close()
		if ret, err := ec.BalanceAt(context.Background(), addr.Common(), base.BiFromBn(bn)); err != nil {
			return (*base.Wei)(ret), err
		} else {
			balance = (*base.Wei)(ret)
		}
	}

	// TODO: BOGUS - THIS IN MEMORY CACHE IS GOOD, BUT COULD BE BINARY FILE
	conn.cacheMutex.Lock()
	conn.balanceCache[key] = balance
	conn.cacheMutex.Unlock()

	return balance, nil
}

func (conn *Connection) getTypeNonProxy(address base.Address, bn base.Blknum) string {
	isContractErr := conn.IsContractAt(address, bn)
	if errors.Is(isContractErr, ErrNotAContract) {
		return "EOA"
	}
	return "Contract"
}
