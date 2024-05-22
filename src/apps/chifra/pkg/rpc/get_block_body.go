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

// TODO: BOGUS - clean raw

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
				toBody := func(block *types.LightBlock) *types.Block {
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

				ret := toBody(lightBlock)
				ret.Transactions = transactions
				// TODO: BOGUS - clean raw - avoid copy
				return *ret, err
			}
		}
	}

	// The block is not in the cache, or reading the cache failed. We
	// need to fetch the block from the RPC.
	rawBlock, err := conn.getRawBlock(bn)
	if err != nil {
		return types.Block{}, err
	}
	isFinal := base.IsFinal(conn.LatestBlockTimestamp, rawBlock.Timestamp)

	_, receiptMap, _ := conn.GetReceiptsByNumber(bn, rawBlock.Timestamp)
	for i := 0; i < len(rawBlock.Transactions); i++ {
		receipt := receiptMap[rawBlock.Transactions[i].TransactionIndex]
		if receipt == nil {
			rec, err := conn.GetReceipt(bn, rawBlock.Transactions[i].TransactionIndex, rawBlock.Timestamp)
			if err != nil {
				return types.Block{}, err
			}
			receipt = &rec
		}
		rawBlock.Transactions[i].GasUsed = receipt.GasUsed
		rawBlock.Transactions[i].IsError = receipt.IsError
		rawBlock.Transactions[i].Receipt = receipt
		rawBlock.Transactions[i].Timestamp = rawBlock.Timestamp
		rawBlock.Transactions[i].HasToken = types.IsTokenFunction(rawBlock.Transactions[i].Input)
		if conn.StoreWritable() && conn.EnabledMap["transactions"] && isFinal {
			_ = conn.Store.Write(&rawBlock.Transactions[i], nil)
		}
	}

	if conn.StoreWritable() && conn.EnabledMap["blocks"] && isFinal {
		_ = conn.Store.Write(rawBlock, nil)
	}

	// TODO: BOGUS - clean raw - avoid copy
	return *rawBlock, nil
}

// getRawBlock returns the raw block as received from the node
func (conn *Connection) getRawBlock(bn base.Blknum) (*types.Block, error) {
	method := "eth_getBlockByNumber"
	params := query.Params{fmt.Sprintf("0x%x", bn), true}

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
