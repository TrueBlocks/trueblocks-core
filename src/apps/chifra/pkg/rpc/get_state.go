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
	blockTs := base.Timestamp(0)
	if conn.StoreReadable() {
		// walk.Cache_State
		state := &types.State{
			BlockNumber: blockNumber,
			Address:     address,
		}
		if err := conn.Store.Read(state, nil); err == nil {
			if state.Parts&fieldBits == fieldBits {
				// we have what we need
				return state, nil
			}
		}
		fieldBits |= state.Parts // preserve what's there
		blockTs = conn.GetBlockTimestamp(blockNumber)
	}

	// We always ask for balance even if we dont' need it. Not sure why.
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

	state := &types.State{
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

	isFinal := base.IsFinal(conn.LatestBlockTimestamp, blockTs)
	if isFinal && conn.StoreWritable() && conn.EnabledMap[walk.Cache_State] {
		_ = conn.Store.Write(state, nil)
	}

	if filters.BalanceCheck != nil {
		if !filters.BalanceCheck(address, &state.Balance) {
			return nil, nil
		}
	}

	return state, nil
}

// GetBalanceAt returns a balance for an address at a block
func (conn *Connection) GetBalanceAt(addr base.Address, bn base.Blknum) (*base.Wei, error) {
	if ec, err := conn.getClient(); err != nil {
		var zero base.Wei
		return &zero, err
	} else {
		defer ec.Close()
		ret, err := ec.BalanceAt(context.Background(), addr.Common(), base.BiFromBn(bn))
		return (*base.Wei)(ret), err
	}
}

func (conn *Connection) getTypeNonProxy(address base.Address, bn base.Blknum) string {
	isContractErr := conn.IsContractAt(address, bn)
	if errors.Is(isContractErr, ErrNotAContract) {
		return "EOA"
	}
	return "Contract"
}
