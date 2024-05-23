// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpc

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

// GetBlockHeaderByNumber fetches the block with only transactions' hashes from the RPC
func (conn *Connection) GetBlockHeaderByNumber(bn base.Blknum) (types.LightBlock, error) {
	if conn.StoreReadable() {
		var block types.LightBlock
		block.BlockNumber = bn
		if err := conn.Store.Read(&block, nil); err == nil {
			// read was successful
			return block, nil
		}
	}

	block, err := conn.getLightBlockFromRpc(bn, notAHash)
	if err != nil {
		return types.LightBlock{}, err
	}

	isFinal := base.IsFinal(conn.LatestBlockTimestamp, block.Timestamp)
	if conn.StoreWritable() && conn.EnabledMap["blocks"] && isFinal {
		_ = conn.Store.Write(block, nil)
	}

	// TODO: BOGUS - avoid copies
	return *block, nil
}

// getLightBlockFromRpc returns the block as received from the node
func (conn *Connection) getLightBlockFromRpc(bn base.Blknum, hash base.Hash) (*types.LightBlock, error) {
	method := "eth_getBlockByNumber"
	params := query.Params{fmt.Sprintf("0x%x", bn), false}
	if hash != notAHash {
		method = "eth_getBlockByHash"
		params = query.Params{hash, false}
	}

	if block, err := query.Query[types.LightBlock](conn.Chain, method, params); err != nil {
		return &types.LightBlock{}, err
	} else {
		block.BlockNumber = block.Number
		if bn == 0 {
			block.Timestamp = conn.GetBlockTimestamp(0)
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
