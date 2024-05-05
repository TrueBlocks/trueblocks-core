// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpc

import (
	"fmt"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetUncleBodiesByNumber returns the number of uncles in a block.
func (conn *Connection) GetUncleBodiesByNumber(bn base.Blknum) ([]types.Block[types.Transaction], error) {
	if count, err := conn.GetUnclesCountInBlock(bn); err != nil {
		return nil, err

	} else if count == 0 {
		return []types.Block[types.Transaction]{}, nil

	} else {
		ret := make([]types.Block[types.Transaction], 0, count)
		for i := uint64(0); i < count; i++ {
			method := "eth_getUncleByBlockNumberAndIndex"
			params := query.Params{
				fmt.Sprintf("0x%x", bn),
				fmt.Sprintf("0x%x", i),
			}

			if rawUncle, err := query.Query[types.RawBlock](conn.Chain, method, params); err != nil {
				return ret, err
			} else {
				// TODO: expand other fields if we ever need them (probably not)
				ret = append(ret, types.Block[types.Transaction]{
					BlockNumber: base.MustParseBlknum(rawUncle.BlockNumber),
					Hash:        base.HexToHash(rawUncle.Hash),
					Miner:       base.HexToAddress(rawUncle.Miner),
					ParentHash:  base.HexToHash(rawUncle.ParentHash),
					Timestamp:   base.MustParseInt(rawUncle.Timestamp),
					// Transactions: rawUncle.Transactions,
					// BaseFeePerGas: rawUncle.BaseFeePerGas,
					// Difficulty: rawUncle.Difficulty,
					// GasLimit: rawUncle.GasLimit,
					// GasUsed: rawUncle.GasUsed,
				})
			}
		}
		return ret, nil
	}
}

// GetUnclesHashesByNumber returns the uncle hashes in a block.
func (conn *Connection) GetUnclesHashesByNumber(bn base.Blknum) ([]base.Hash, error) {
	if count, err := conn.GetUnclesCountInBlock(bn); err != nil {
		return nil, err
	} else if count == 0 {
		return []base.Hash{}, nil
	} else {
		ret := make([]base.Hash, 0, count)
		for i := uint64(0); i < count; i++ {
			method := "eth_getUncleByBlockNumberAndIndex"
			params := query.Params{
				fmt.Sprintf("0x%x", bn),
				fmt.Sprintf("0x%x", i),
			}
			if rawUncle, err := query.Query[types.RawBlock](conn.Chain, method, params); err != nil {
				return ret, err
			} else {
				for _, uncle := range rawUncle.Uncles {
					ret = append(ret, base.HexToHash(uncle))
				}
			}
		}
		return ret, nil
	}
}

// GetUnclesCountInBlock returns the number of uncles in a block.
func (conn *Connection) GetUnclesCountInBlock(bn base.Blknum) (uint64, error) {
	if bn >= base.KnownBlock(conn.Chain, base.Merge) {
		return 0, nil
	}

	method := "eth_getUncleCountByBlockNumber"
	params := query.Params{fmt.Sprintf("0x%x", bn)}

	if count, err := query.Query[string](conn.Chain, method, params); err != nil {
		return 0, err
	} else if count == nil || *count == "" {
		return 0, nil
	} else {
		return strconv.ParseUint(fmt.Sprint(*count), 0, 64)
	}
}
