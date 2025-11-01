// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpc

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/walk"
	"github.com/ethereum/go-ethereum"
)

// GetBlockHeaderByNumber fetches the block with only transactions' hashes from the RPC
func (conn *Connection) GetBlockHeaderByNumber(bn base.Blknum) (types.LightBlock, error) {
	if bn != base.NOPOSN {
		block := types.LightBlock{
			BlockNumber: bn,
		}
		if err := conn.ReadFromCache(&block); err == nil {
			return block, nil
		}
	}

	if block, err := conn.getLightBlockFromRpc(bn, notAHash); err != nil {
		return types.LightBlock{}, err
	} else {
		err = conn.WriteToCache(block, walk.Cache_Blocks, block.Timestamp)
		return *block, err
	}
}

// getLightBlockFromRpc returns the block as received from the node
func (conn *Connection) getLightBlockFromRpc(bn base.Blknum, hash base.Hash) (*types.LightBlock, error) {
	method := "eth_getBlockByNumber"
	params := query.Params{fmt.Sprintf("0x%x", bn), false}
	if bn == base.NOPOSN {
		params = query.Params{"latest", false}
	}
	if !hash.IsZero() {
		method = "eth_getBlockByHash"
		params = query.Params{hash, false}
	}

	if block, err := query.Query[types.LightBlock](conn.Chain, method, params); err != nil {
		return &types.LightBlock{}, err
	} else {
		block.BlockNumber = block.Number
		if bn == 0 {
			// TODO: Chain specific
			block.Timestamp = conn.GetBlockTimestamp(1) - 13
		} else if block.Timestamp == 0 {
			return &types.LightBlock{}, fmt.Errorf("block at %s returned an error: %w", fmt.Sprintf("%d", bn), ethereum.NotFound)
		}
		for i := 0; i < len(block.Withdrawals); i++ {
			block.Withdrawals[i].BlockNumber = block.BlockNumber
			block.Withdrawals[i].Timestamp = block.Timestamp
		}
		return block, nil
	}
}
