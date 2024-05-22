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

	block, rawBlock, err := loadBlockFull(conn, bn)
	if err != nil {
		return block, err
	}

	ts := rawBlock.Timestamp
	block.Transactions = make([]types.Transaction, 0, len(rawBlock.Transactions))
	_, receiptMap, _ := conn.GetReceiptsByNumber(bn, ts)
	for _, rawTx := range rawBlock.Transactions {
		idx := rawTx.TransactionIndex
		var receipt types.Receipt
		if receiptMap[idx] == nil {
			receipt, err = conn.GetReceipt(bn, idx, ts)
			if err != nil {
				return block, err
			}
		} else {
			receipt = *receiptMap[idx]
		}

		tx := NewTransaction2(&rawTx, &receipt, ts)
		block.Transactions = append(block.Transactions, *tx)

		if conn.StoreWritable() && conn.EnabledMap["transactions"] && base.IsFinal(conn.LatestBlockTimestamp, tx.Timestamp) {
			_ = conn.Store.Write(tx, nil)
		}
	}

	if conn.StoreWritable() && conn.EnabledMap["blocks"] && base.IsFinal(conn.LatestBlockTimestamp, block.Timestamp) {
		_ = conn.Store.Write(&block, nil)
	}

	return block, nil
}

// NewTransaction2 builds Transaction using data from raw and receipt. Receipt can be nil.
// Transaction timestamp and HasToken flag will be set to timestamp and hasToken.
func NewTransaction2(raw *types.Transaction, receipt *types.Receipt, timestamp base.Timestamp) *types.Transaction {
	s := *raw
	s.Timestamp = timestamp
	s.HasToken = types.IsTokenFunction(raw.Input)
	s.GasUsed = receipt.GasUsed
	s.IsError = receipt.IsError
	s.Receipt = receipt
	return &s
}

// loadBlockFull fetches block from RPC with full transactions.
func loadBlockFull(conn *Connection, bn base.Blknum) (types.Block, *types.Block, error) {
	block, err := conn.getRawBlock(bn)
	if err != nil {
		return types.Block{}, nil, err
	}
	return *block, block, err
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
