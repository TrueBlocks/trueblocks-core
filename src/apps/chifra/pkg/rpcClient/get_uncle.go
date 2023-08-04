// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"fmt"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// GetCountUnclesInBlock returns the number of uncles in a block.
func (conn *Connection) GetCountUnclesInBlock(bn uint64) (uint64, error) {
	method := "eth_getUncleCountByBlockNumber"
	params := rpc.Params{fmt.Sprintf("0x%x", bn)}

	if count, err := rpc.Query[string](conn.Chain, method, params); err != nil {
		return 0, err
	} else {
		return strconv.ParseUint(fmt.Sprint(*count), 0, 64)
	}
}

// GetUncleHashesByNumber returns the uncle hashes in a block.
func (conn *Connection) GetUncleHashesByNumber(bn uint64) ([]base.Hash, error) {
	if count, err := conn.GetCountUnclesInBlock(bn); err != nil {
		return nil, err
	} else if count > 0 {
		ret := make([]base.Hash, count)
		for i := uint64(0); i < count; i++ {
			method := "eth_getUncleByBlockNumberAndIndex"
			params := rpc.Params{
				fmt.Sprintf("0x%x", bn),
				fmt.Sprintf("0x%x", i),
			}
			if rawUncle, err := rpc.Query[types.RawBlock](conn.Chain, method, params); err != nil {
				return ret, err
			} else {
				for _, uncle := range rawUncle.Uncles {
					ret = append(ret, base.HexToHash(uncle))
				}
			}
		}
		return ret, nil
	} else {
		return []base.Hash{}, nil
	}
}

// GetUnclesByNumber returns the number of uncles in a block.
func (conn *Connection) GetUnclesByNumber(bn uint64) ([]types.SimpleBlock[types.SimpleTransaction], error) {
	if count, err := conn.GetCountUnclesInBlock(bn); err != nil {
		return nil, err
	} else if count > 0 {
		ret := make([]types.SimpleBlock[types.SimpleTransaction], count)
		for i := uint64(0); i < count; i++ {
			method := "eth_getUncleByBlockNumberAndIndex"
			params := rpc.Params{
				fmt.Sprintf("0x%x", bn),
				fmt.Sprintf("0x%x", i),
			}

			if rawUncle, err := rpc.Query[types.RawBlock](conn.Chain, method, params); err != nil {
				return ret, err
			} else {
				// TODO: expand other fields if we ever need them (probably not)
				ret = append(ret, types.SimpleBlock[types.SimpleTransaction]{
					BlockNumber: utils.MustParseUint(rawUncle.BlockNumber),
					Hash:        base.HexToHash(rawUncle.Hash),
					Miner:       base.HexToAddress(rawUncle.Miner),
					ParentHash:  base.HexToHash(rawUncle.ParentHash),
					Timestamp:   int64(utils.MustParseUint(rawUncle.Timestamp)),
					// Transactions: rawUncle.Transactions,
					// BaseFeePerGas: rawUncle.BaseFeePerGas,
					// Difficulty: rawUncle.Difficulty,
					// GasLimit: rawUncle.GasLimit,
					// GasUsed: rawUncle.GasUsed,
				})
			}
		}
		return ret, nil
	} else {
		return []types.SimpleBlock[types.SimpleTransaction]{}, nil
	}
}
