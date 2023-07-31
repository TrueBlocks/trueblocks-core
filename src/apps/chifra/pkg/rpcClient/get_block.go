// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"context"
	"errors"
	"fmt"
	"math/big"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// GetBlockBodyByNumber fetches the block with transactions from the RPC.
func GetBlockBodyByNumber(chain string, bn uint64, options *Options) (types.SimpleBlock[types.SimpleTransaction], error) {
	if options.HasStore() {
		// We only cache blocks with transaction hashes
		cachedBlock := types.SimpleBlock[string]{BlockNumber: bn}
		if err := options.Store.Read(&cachedBlock, nil); err == nil {
			// Success, we now have to fill in transaction objects
			result := types.SimpleBlock[types.SimpleTransaction]{}
			result.Transactions = make([]types.SimpleTransaction, 0, len(cachedBlock.Transactions))
			success := true
			for index := range cachedBlock.Transactions {
				tx, err := GetTransactionByBlockAndId(chain, cachedBlock.BlockNumber, uint64(index), options)
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

	block, rawBlock, err := loadBlock[types.SimpleTransaction](chain, bn, true)
	block.SetRaw(rawBlock) // may have failed, but it's ok
	if err != nil {
		return block, err
	}

	var writeOptions *cacheNew.WriteOptions
	if options.HasStoreWritable() {
		writeOptions = &cacheNew.WriteOptions{
			// Check if the block is final
			Pending: block.Pending(options.LatestBlockTimestamp),
		}
	}

	block.Uncles = make([]base.Hash, 0, len(rawBlock.Uncles))
	for _, uncle := range rawBlock.Uncles {
		block.Uncles = append(block.Uncles, base.HexToHash(uncle))
	}

	ts, _ := strconv.ParseInt(rawBlock.Timestamp, 0, 64)
	block.Transactions = make([]types.SimpleTransaction, 0, len(rawBlock.Transactions))
	for _, rawTx := range rawBlock.Transactions {
		// cast transaction to a concrete type
		rawData, ok := rawTx.(map[string]any)
		if !ok {
			err = errors.New("cannot cast raw block transaction into map")
			return block, err
		}
		raw := types.NewRawTransactionFromMap(rawData)

		// Get the receipt
		var receipt types.SimpleReceipt
		receipt, err = GetReceipt(chain, ReceiptQuery{
			Bn:       uint64(bn),
			Txid:     uint64(raw.TxIndex()),
			TxHash:   raw.TxHash(),
			GasPrice: raw.TxGasPrice(),
			NeedsTs:  true,
			Ts:       ts,
		}, options)
		if err != nil {
			return block, err
		}

		tx := types.NewSimpleTransaction(raw, &receipt, ts)
		block.Transactions = append(block.Transactions, *tx)

		if options.HasStore() && !options.TransactionWriteDisabled {
			options.Store.Write(tx, writeOptions)
		}
	}

	if options.HasStore() {
		options.Store.Write(&block, writeOptions)
	}
	return block, nil
}

// GetBlockHeaderByNumber fetches the block with only transactions' hashes from the RPC
func GetBlockHeaderByNumber(chain string, bn uint64, options *Options) (block types.SimpleBlock[string], err error) {
	if options.HasStore() {
		block.BlockNumber = bn
		if err := options.Store.Read(&block, nil); err == nil {
			// Success
			return block, nil
		}
	}

	block, rawBlock, err := loadBlock[string](chain, bn, false)
	block.SetRaw(rawBlock) // may have failed, but it's ok
	if err != nil {
		return block, err
	}

	block.Uncles = make([]base.Hash, 0, len(rawBlock.Uncles))
	for _, uncle := range rawBlock.Uncles {
		block.Uncles = append(block.Uncles, base.HexToHash(uncle))
	}

	block.Transactions = make([]string, 0, len(rawBlock.Transactions))
	for _, rawTx := range rawBlock.Transactions {
		block.Transactions = append(block.Transactions, fmt.Sprint(rawTx))
	}

	if options.HasStoreWritable() {
		writeOptions := &cacheNew.WriteOptions{
			// Check if the block is final
			Pending: block.Pending(options.LatestBlockTimestamp),
		}
		options.Store.Write(&block, writeOptions)
	}

	return block, nil
}

// loadBlock fetches block from RPC, but it does not try to fill Transactions field. This is delegated to
// more specialized functions and makes loadBlock generic.
func loadBlock[Tx string | types.SimpleTransaction](chain string, bn uint64, withTxs bool) (block types.SimpleBlock[Tx], rawBlock *types.RawBlock, err error) {
	rawBlock, err = getRawBlock(chain, bn, withTxs)
	if err != nil {
		return
	}

	ts, err := hexutil.DecodeUint64(rawBlock.Timestamp)
	if err != nil {
		return
	}

	blockNumber, err := hexutil.DecodeUint64(rawBlock.BlockNumber)
	if err != nil {
		return
	}

	gasLimit, err := hexutil.DecodeUint64(rawBlock.GasLimit)
	if err != nil {
		return
	}

	gasUsed, err := hexutil.DecodeUint64(rawBlock.GasUsed)
	if err != nil {
		return
	}

	difficulty, err := hexutil.DecodeUint64(rawBlock.Difficulty)
	if err != nil {
		return
	}

	uncles := make([]base.Hash, 0, len(rawBlock.Uncles))
	for _, uncle := range rawBlock.Uncles {
		uncles = append(uncles, base.HexToHash(uncle))
	}

	block = types.SimpleBlock[Tx]{
		BlockNumber: blockNumber,
		Timestamp:   base.Timestamp(ts), // note that we turn Ethereum's timestamps into types.Timestamp upon read.
		Hash:        base.HexToHash(rawBlock.Hash),
		ParentHash:  base.HexToHash(rawBlock.ParentHash),
		GasLimit:    gasLimit,
		GasUsed:     gasUsed,
		Miner:       base.HexToAddress(rawBlock.Miner),
		Difficulty:  difficulty,
		Uncles:      uncles,
	}
	return
}

func getRawBlock(chain string, bn uint64, withTxs bool) (*types.RawBlock, error) {
	method := "eth_getBlockByNumber"
	params := rpc.Params{fmt.Sprintf("0x%x", bn), withTxs}

	if block, err := rpc.Query[types.RawBlock](chain, method, params); err != nil {
		return &types.RawBlock{}, err
	} else {
		if bn == 0 {
			// The RPC does not return a timestamp for the zero block, so we make one
			block.Timestamp = fmt.Sprintf("0x%x", GetBlockTimestamp(chain, utils.PointerOf(uint64(0))))
		} else if utils.MustParseUint(block.Timestamp) == 0 {
			return &types.RawBlock{}, fmt.Errorf("block at %s returned an error: %w", fmt.Sprintf("%d", bn), ethereum.NotFound)
		}

		return block, nil
	}
}

//---------------------------------------------------------------------------

// GetBlockTimestamp returns the timestamp associated with a given block
func GetBlockTimestamp(chain string, bn *uint64) base.Timestamp {
	provider, _ := config.GetRpcProvider(chain)
	if ec := GetClient(provider); ec == nil {
		logger.Error("Could not connect to RPC client")
		return 0
	} else {
		defer ec.Close()

		var blockNumber *big.Int
		if bn != nil {
			blockNumber = big.NewInt(int64(*bn))
		}

		r, err := ec.HeaderByNumber(context.Background(), blockNumber)
		if err != nil {
			logger.Error("Could not connect to RPC client", err)
			return 0
		}

		ts := base.Timestamp(r.Time)
		if ts == 0 {
			// The RPC does not return a timestamp for block zero, so we simulate it with ts from block one less 13 seconds
			// TODO: Chain specific
			return GetBlockTimestamp(chain, utils.PointerOf(uint64(1))) - 13
		}

		return ts
	}
}
