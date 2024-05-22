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
	rawBlock, err := loadBlockFull(conn, bn)
	block := *rawBlock
	if err != nil {
		return block, err
	}

	transactions := make([]types.Transaction, 0, len(rawBlock.Transactions))
	isFinal := base.IsFinal(conn.LatestBlockTimestamp, block.Timestamp)
	_, receiptMap, _ := conn.GetReceiptsByNumber(bn, block.Timestamp)
	for _, rawTx := range rawBlock.Transactions {
		idx := rawTx.TransactionIndex
		var receipt types.Receipt
		if receiptMap[idx] == nil {
			receipt, err = conn.GetReceipt(bn, idx, block.Timestamp)
			if err != nil {
				return block, err
			}
		} else {
			receipt = *receiptMap[idx]
		}

		tx := rawTx
		tx.Timestamp = block.Timestamp
		tx.HasToken = types.IsTokenFunction(rawTx.Input)
		tx.GasUsed = receipt.GasUsed
		tx.IsError = receipt.IsError
		tx.Receipt = &receipt
		transactions = append(transactions, tx)

		if conn.StoreWritable() && conn.EnabledMap["transactions"] && isFinal {
			_ = conn.Store.Write(&tx, nil)
		}
	}

	block.Transactions = transactions
	if conn.StoreWritable() && conn.EnabledMap["blocks"] && isFinal {
		_ = conn.Store.Write(&block, nil)
	}

	return block, nil
}

// loadBlockFull fetches block from RPC with full transactions.
func loadBlockFull(conn *Connection, bn base.Blknum) (*types.Block, error) {
	block, err := conn.getRawBlock(bn)
	if err != nil {
		return nil, err
	}
	return block, nil
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
