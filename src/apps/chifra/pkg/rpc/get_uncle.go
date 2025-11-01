// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpc

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
)

// GetUncleBodiesByNumber returns the number of uncles in a block. (search: FromRpc)
func (conn *Connection) GetUncleBodiesByNumber(bn base.Blknum) ([]types.Block, error) {
	if count, err := conn.GetUnclesCountInBlock(bn); err != nil {
		return nil, err

	} else if count == 0 {
		return []types.Block{}, nil

	} else {
		ret := make([]types.Block, 0, count)
		for i := uint64(0); i < count; i++ {
			method := "eth_getUncleByBlockNumberAndIndex"
			params := query.Params{
				fmt.Sprintf("0x%x", bn),
				fmt.Sprintf("0x%x", i),
			}

			if uncle, err := query.Query[types.Block](conn.Chain, method, params); err != nil {
				return ret, err
			} else {
				uncle.BlockNumber = uncle.Number
				ret = append(ret, *uncle)
			}
		}
		// TODO: BOGUS - avoid copy
		return ret, nil
	}
}

// GetUnclesCountInBlock returns the number of uncles in a block. (search: FromRpc)
func (conn *Connection) GetUnclesCountInBlock(bn base.Blknum) (uint64, error) {
	if bn >= base.KnownBlock(conn.Chain, base.Merge) {
		return 0, nil
	}

	method := "eth_getUncleCountByBlockNumber"
	params := query.Params{fmt.Sprintf("0x%x", bn)}

	if count, err := query.Query[base.Value](conn.Chain, method, params); err != nil {
		return 0, err
	} else {
		return uint64(*count), nil
	}
}
