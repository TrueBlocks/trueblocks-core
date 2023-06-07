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
)

// GetUncleCountByNumber returns the number of uncles in a block.
func GetUncleCountByNumber(chain string, bn uint64) (uint64, error) {
	method := "eth_getUncleCountByBlockNumber"
	params := rpc.Params{fmt.Sprintf("0x%x", bn)}

	if count, err := rpc.Query[string](chain, method, params); err != nil {
		return 0, err
	} else {
		return strconv.ParseUint(fmt.Sprint(*count), 0, 64)
	}
}

// GetUnclesByNumber returns the number of uncles in a block.
func GetUnclesByNumber(chain string, bn uint64) ([]types.SimpleBlock[types.SimpleTransaction], error) {
	if count, err := GetUncleCountByNumber(chain, bn); err != nil {
		return nil, err
	} else {
		ret := make([]types.SimpleBlock[types.SimpleTransaction], count)
		for i := uint64(0); i < count; i++ {
			method := "eth_getUncleByBlockNumberAndIndex"
			params := rpc.Params{fmt.Sprintf("0x%x, 0x%x", bn, i)}

			if rawUncle, err := rpc.Query[types.RawBlock](chain, method, params); err != nil {
				return ret, err
			} else {
				uncles := make([]base.Hash, len(rawUncle.Uncles))
				for i, uncle := range rawUncle.Uncles {
					uncles[i] = base.HexToHash(uncle)
				}
				// TODO: expand other fields if we ever need them (probably not)
				ret = append(ret, types.SimpleBlock[types.SimpleTransaction]{
					// BaseFeePerGas: rawUncle.BaseFeePerGas,
					BlockNumber: mustParseUint(rawUncle.BlockNumber),
					// Difficulty: rawUncle.Difficulty,
					// GasLimit: rawUncle.GasLimit,
					// GasUsed: rawUncle.GasUsed,
					Hash:       base.HexToHash(rawUncle.Hash),
					Miner:      base.HexToAddress(rawUncle.Miner),
					ParentHash: base.HexToHash(rawUncle.ParentHash),
					Timestamp:  int64(mustParseUint(rawUncle.Timestamp)),
					// Transactions: rawUncle.Transactions,
					Uncles: uncles,
				})
			}
		}
		return ret, nil
	}
}
