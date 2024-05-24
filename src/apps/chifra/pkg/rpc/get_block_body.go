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

// GetBlockBodyByNumber fetches the block with transactions from the RPC.
func (conn *Connection) GetBlockBodyByNumber(bn base.Blknum) (types.Block, error) {
	var err error
	if conn.StoreReadable() {
		lightBlock := &types.LightBlock{BlockNumber: bn}
		if err := conn.Store.Read(lightBlock, nil); err == nil {
			// We need to fill in the actual transactions (from cache hopefully, but
			// if not, then from the RPC)
			transactions := make([]types.Transaction, 0, len(lightBlock.Transactions))
			for index := range lightBlock.Transactions {
				tx, thisErr := conn.GetTransactionByNumberAndId(lightBlock.BlockNumber, base.Txnum(index))
				if thisErr != nil {
					err = thisErr
					break
				}
				transactions = append(transactions, *tx)
			}

			if err == nil && len(transactions) == len(lightBlock.Transactions) {
				lightToBody := func(block *types.LightBlock) *types.Block {
					var ret types.Block
					ret.BaseFeePerGas = block.BaseFeePerGas
					ret.BlockNumber = block.BlockNumber
					ret.Difficulty = block.Difficulty
					ret.GasLimit = block.GasLimit
					ret.GasUsed = block.GasUsed
					ret.Hash = block.Hash
					ret.Miner = block.Miner
					ret.ParentHash = block.ParentHash
					ret.Timestamp = block.Timestamp
					ret.Uncles = block.Uncles
					ret.Withdrawals = block.Withdrawals
					return &ret
				}

				ret := lightToBody(lightBlock)
				ret.Transactions = transactions
				// TODO: BOGUS - avoid copy
				return *ret, err
			}
		}
	}

	// The block is not in the cache, or reading the cache failed. We
	// need to fetch the block from the RPC.
	block, err := conn.getBlockFromRpc(bn, notAHash)
	if err != nil {
		return types.Block{}, err
	}
	isFinal := base.IsFinal(conn.LatestBlockTimestamp, block.Timestamp)

	_, receiptMap, _ := conn.GetReceiptsByNumber(bn, block.Timestamp)
	for i := 0; i < len(block.Transactions); i++ {
		receipt := receiptMap[block.Transactions[i].TransactionIndex]
		if receipt == nil {
			rec, err := conn.GetReceipt(bn, block.Transactions[i].TransactionIndex, block.Timestamp)
			if err != nil {
				return types.Block{}, err
			}
			receipt = &rec
		}
		block.Transactions[i].GasUsed = receipt.GasUsed
		block.Transactions[i].IsError = receipt.IsError
		block.Transactions[i].Receipt = receipt
		block.Transactions[i].Timestamp = block.Timestamp
		block.Transactions[i].HasToken = types.IsTokenFunction(block.Transactions[i].Input)
		if conn.StoreWritable() && conn.EnabledMap["transactions"] && isFinal {
			_ = conn.Store.Write(&block.Transactions[i], nil)
		}
	}

	if conn.StoreWritable() && conn.EnabledMap["blocks"] && isFinal {
		_ = conn.Store.Write(block, nil)
	}

	// TODO: BOGUS - avoid copy
	return *block, nil
}

// getBlockFromRpc returns the block as received from the node
func (conn *Connection) getBlockFromRpc(bn base.Blknum, hash base.Hash) (*types.Block, error) {
	method := "eth_getBlockByNumber"
	params := query.Params{fmt.Sprintf("0x%x", bn), true}
	if hash != notAHash {
		method = "eth_getBlockByHash"
		params = query.Params{hash, true}
	}

	if block, err := query.Query[types.Block](conn.Chain, method, params); err != nil {
		return &types.Block{}, err
	} else {
		block.BlockNumber = block.Number
		if bn == 0 {
			block.Timestamp = conn.GetBlockTimestamp(0)
		} else if block.Timestamp == 0 {
			return &types.Block{}, fmt.Errorf("block at %s returned an error: %w", fmt.Sprintf("%d", bn), ethereum.NotFound)
		}
		for i := 0; i < len(block.Withdrawals); i++ {
			block.Withdrawals[i].BlockNumber = block.BlockNumber
			block.Withdrawals[i].Timestamp = block.Timestamp
		}
		return block, nil
	}
}
