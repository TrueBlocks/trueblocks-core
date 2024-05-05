// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpc

import (
	"context"
	"errors"
	"fmt"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum"
	"github.com/ethereum/go-ethereum/common"
)

// GetBlockBodyByNumber fetches the block with transactions from the RPC.
func (conn *Connection) GetBlockBodyByNumber(bn base.Blknum) (types.Block[types.Transaction], error) {
	if conn.StoreReadable() {
		// We only cache blocks with transaction hashes
		cachedBlock := types.Block[string]{BlockNumber: bn}
		if err := conn.Store.Read(&cachedBlock, nil); err == nil {
			// Success, we now have to fill in transaction objects
			result := types.Block[types.Transaction]{}
			result.Transactions = make([]types.Transaction, 0, len(cachedBlock.Transactions))
			success := true
			for index := range cachedBlock.Transactions {
				tx, err := conn.GetTransactionByNumberAndId(cachedBlock.BlockNumber, base.Txnum(index))
				if err != nil {
					success = false
					break
				}
				result.Transactions = append(result.Transactions, *tx)
			}
			if success {
				cachedBlock.Dup(&result)
				return result, nil
			}
		}
	}

	block, rawBlock, err := loadBlock[types.Transaction](conn, bn, true)
	block.SetRaw(rawBlock) // may have failed, but it's ok
	if err != nil {
		return block, err
	}

	ts, _ := strconv.ParseInt(rawBlock.Timestamp, 0, 64)
	block.Transactions = make([]types.Transaction, 0, len(rawBlock.Transactions))
	_, receiptMap, _ := conn.GetReceiptsByNumber(bn, ts)
	for _, rawTx := range rawBlock.Transactions {
		// cast transaction to a concrete type
		rawData, ok := rawTx.(map[string]any)
		if !ok {
			err = errors.New("cannot cast raw block transaction into map")
			return block, err
		}
		raw := types.NewRawTransactionFromMap(rawData)

		// Get the receipt
		idx := base.MustParseNumeral(raw.TransactionIndex)
		var receipt types.Receipt
		if receiptMap[idx] == nil {
			receipt, err = conn.GetReceipt(bn, idx, ts)
			if err != nil {
				return block, err
			}
		} else {
			receipt = *receiptMap[idx]
		}

		tx := types.NewTransaction(raw, &receipt, ts)
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

// GetBlockHeaderByNumber fetches the block with only transactions' hashes from the RPC
func (conn *Connection) GetBlockHeaderByNumber(bn base.Blknum) (block types.Block[string], err error) {
	if conn.StoreReadable() {
		block.BlockNumber = bn
		if err := conn.Store.Read(&block, nil); err == nil {
			return block, nil
		}
	}

	block, rawBlock, err := loadBlock[string](conn, bn, false)
	block.SetRaw(rawBlock) // may have failed, but it's ok
	if err != nil {
		return block, err
	}

	block.Transactions = make([]string, 0, len(rawBlock.Transactions))
	for _, txHash := range rawBlock.Transactions {
		block.Transactions = append(block.Transactions, fmt.Sprint(txHash))
	}

	if conn.StoreWritable() && conn.EnabledMap["blocks"] && base.IsFinal(conn.LatestBlockTimestamp, block.Timestamp) {
		_ = conn.Store.Write(&block, nil)
	}

	return block, nil
}

// GetBlockTimestamp returns the timestamp associated with a given block
func (conn *Connection) GetBlockTimestamp(bn base.Blknum) base.Timestamp {
	if ec, err := conn.getClient(); err != nil {
		logger.Error("Could not connect to RPC client", err)
		return 0
	} else {
		defer ec.Close()

		r, err := ec.HeaderByNumber(context.Background(), base.BiFromBn(bn))
		if err != nil {
			logger.Error("Could not connect to RPC client", err)
			return 0
		}

		ts := base.Timestamp(r.Time)
		if ts == 0 {
			// The RPC does not return a timestamp for block zero, so we simulate it with ts from block one less 13 seconds
			// TODO: Chain specific
			return conn.GetBlockTimestamp(1) - 13
		}

		return ts
	}
}

// GetBlockHashByHash returns a block's hash if it's a valid block
func (conn *Connection) GetBlockHashByHash(hash string) (base.Hash, error) {
	if ec, err := conn.getClient(); err != nil {
		return base.Hash{}, err
	} else {
		defer ec.Close()

		ethBlock, err := ec.BlockByHash(context.Background(), common.HexToHash(hash))
		if err != nil {
			return base.Hash{}, err
		}

		return base.HexToHash(ethBlock.Hash().Hex()), nil
	}
}

// GetBlockNumberByHash returns a block's hash if it's a valid block
func (conn *Connection) GetBlockNumberByHash(hash string) (base.Blknum, error) {
	if ec, err := conn.getClient(); err != nil {
		return 0, err
	} else {
		defer ec.Close()

		ethBlock, err := ec.BlockByHash(context.Background(), common.HexToHash(hash))
		if err != nil {
			return 0, err
		}

		return base.Blknum(ethBlock.NumberU64()), nil
	}
}

// GetBlockHashByNumber returns a block's hash if it's a valid block
func (conn *Connection) GetBlockHashByNumber(bn base.Blknum) (base.Hash, error) {
	if ec, err := conn.getClient(); err != nil {
		return base.Hash{}, err
	} else {
		defer ec.Close()

		ethBlock, err := ec.BlockByNumber(context.Background(), base.BiFromBn(bn))
		if err != nil {
			return base.Hash{}, err
		}

		return base.HexToHash(ethBlock.Hash().Hex()), nil
	}
}

// loadBlock fetches block from RPC, but it does not try to fill Transactions field. This is delegated to
// more specialized functions and makes loadBlock generic.
func loadBlock[Tx string | types.Transaction](conn *Connection, bn base.Blknum, withTxs bool) (block types.Block[Tx], rawBlock *types.RawBlock, err error) {
	rawBlock, err = conn.getBlockRaw(bn, withTxs)
	if err != nil {
		return
	}

	uncleHashes := make([]base.Hash, 0, len(rawBlock.Uncles))
	for _, uncle := range rawBlock.Uncles {
		uncleHashes = append(uncleHashes, base.HexToHash(uncle))
	}

	block = types.Block[Tx]{
		BlockNumber: base.MustParseBlknum(rawBlock.BlockNumber),
		Timestamp:   base.Timestamp(utils.MustParseInt(rawBlock.Timestamp)), // note that we turn Ethereum's timestamps into types. Timestamp upon read.
		Hash:        base.HexToHash(rawBlock.Hash),
		ParentHash:  base.HexToHash(rawBlock.ParentHash),
		GasLimit:    base.MustParseNumeral(rawBlock.GasLimit),
		GasUsed:     base.MustParseNumeral(rawBlock.GasUsed),
		Miner:       base.HexToAddress(rawBlock.Miner),
		Difficulty:  utils.MustParseUint(rawBlock.Difficulty),
		Uncles:      uncleHashes,
	}

	if len(rawBlock.Withdrawals) > 0 {
		block.Withdrawals = make([]types.Withdrawal, 0, len(rawBlock.Withdrawals))
		for _, withdrawal := range rawBlock.Withdrawals {
			amt := base.NewWei(0)
			amt.SetString(withdrawal.Amount, 0)
			s := types.Withdrawal{
				Address:        base.HexToAddress(withdrawal.Address),
				Amount:         *amt,
				BlockNumber:    block.BlockNumber,
				Timestamp:      block.Timestamp,
				Index:          base.MustParseNumeral(withdrawal.Index),
				ValidatorIndex: base.MustParseNumeral(withdrawal.ValidatorIndex),
			}
			block.Withdrawals = append(block.Withdrawals, s)
		}
	}

	return
}

// getBlockRaw returns the raw block as received from the node
func (conn *Connection) getBlockRaw(bn base.Blknum, withTxs bool) (*types.RawBlock, error) {
	method := "eth_getBlockByNumber"
	params := query.Params{fmt.Sprintf("0x%x", bn), withTxs}

	if block, err := query.Query[types.RawBlock](conn.Chain, method, params); err != nil {
		return &types.RawBlock{}, err
	} else {
		if bn == 0 {
			// The RPC does not return a timestamp for the zero block, so we make one
			block.Timestamp = fmt.Sprintf("0x%x", conn.GetBlockTimestamp(0))
		} else if utils.MustParseUint(block.Timestamp) == 0 {
			return &types.RawBlock{}, fmt.Errorf("block at %s returned an error: %w", fmt.Sprintf("%d", bn), ethereum.NotFound)
		}

		return block, nil
	}
}

// This most likely does not work for non-mainnet chains which don't know
// anything about the Known blocks.

// getBlockReward returns the block reward for a given block number
func (conn *Connection) getBlockReward(bn base.Blknum) *base.Wei {
	if bn == 0 {
		return base.NewWei(0)
	} else if bn < base.KnownBlock(conn.Chain, base.Byzantium) {
		return base.NewWei(5000000000000000000)
	} else if bn < base.KnownBlock(conn.Chain, base.Constantinople) {
		return base.NewWei(3000000000000000000)
	} else if bn < base.KnownBlock(conn.Chain, base.Merge) {
		return base.NewWei(2000000000000000000)
	} else {
		return base.NewWei(0)
	}
}
