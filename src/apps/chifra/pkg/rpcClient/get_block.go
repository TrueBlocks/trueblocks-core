// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"errors"
	"fmt"
	"math/big"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

func GetBlockHeaderByNumber(chain string, bn uint64) (types.SimpleBlock[string], error) {
	return GetBlockByNumber(chain, bn, nil)
}

// GetBlockByNumberWithTxs fetches the block with transactions from the RPC.
func GetBlockByNumberWithTxs(chain string, bn uint64, cache *cacheNew.Store) (types.SimpleBlock[types.SimpleTransaction], error) {
	if cache != nil {
		// We only cache blocks with transaction hashes
		cachedBlock := types.SimpleBlock[string]{BlockNumber: bn}
		if err := cache.Read(&cachedBlock, nil); err == nil {
			// Success, we now have to fill in transaction objects
			result := types.SimpleBlock[types.SimpleTransaction]{}
			result.Transactions = make([]types.SimpleTransaction, 0, len(cachedBlock.Transactions))
			success := true
			for index := range cachedBlock.Transactions {
				tx, err := GetTransactionByBlockAndId(chain, cachedBlock.BlockNumber, uint64(index), cache)
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

	block.Uncles = make([]base.Hash, 0, len(rawBlock.Uncles))
	for _, uncle := range rawBlock.Uncles {
		block.Uncles = append(block.Uncles, base.HexToHash(uncle))
	}

	ts, _ := strconv.ParseInt(rawBlock.Timestamp, 0, 64)
	block.Transactions = make([]types.SimpleTransaction, 0, len(rawBlock.Transactions))
	for _, rawTx := range rawBlock.Transactions {
		// cast transaction to a concrete type
		t, ok := rawTx.(map[string]any)
		if !ok {
			err = errors.New("cannot cast raw block transaction into map")
			return block, err
		}

		txHash := base.HexToHash(fmt.Sprint(t["hash"]))
		txGasPrice := mustParseUint(t["gasPrice"])
		input := fmt.Sprint(t["input"])
		value := big.NewInt(0)
		value.SetString(fmt.Sprint(t["value"]), 0)
		txIndex := mustParseUint(t["transactionIndex"])
		hasToken := IsTokenRelated(input)

		// Get the receipt
		var receipt types.SimpleReceipt
		receipt, err = GetTransactionReceipt(chain, ReceiptQuery{
			Bn:       uint64(bn),
			Txid:     uint64(txIndex),
			TxHash:   &txHash,
			GasPrice: txGasPrice,
			NeedsTs:  true,
			Ts:       ts,
		})
		if err != nil {
			return block, err
		}

		tx := types.SimpleTransaction{
			Hash:                 txHash,
			BlockHash:            base.HexToHash(fmt.Sprint(t["blockHash"])),
			BlockNumber:          mustParseUint(t["blockNumber"]),
			TransactionIndex:     mustParseUint(t["transactionIndex"]),
			Nonce:                mustParseUint(t["nonce"]),
			Timestamp:            ts,
			From:                 base.HexToAddress(fmt.Sprint(t["from"])),
			To:                   base.HexToAddress(fmt.Sprint(t["to"])),
			Value:                *value,
			Gas:                  mustParseUint(t["gas"]),
			GasPrice:             txGasPrice,
			GasUsed:              receipt.GasUsed,
			MaxFeePerGas:         mustParseUint(t["maxFeePerGas"]),
			MaxPriorityFeePerGas: mustParseUint(t["maxPriorityFeePerGas"]),
			Input:                input,
			IsError:              receipt.IsError,
			HasToken:             hasToken,
			Receipt:              &receipt,
			// Traces               []SimpleTrace   `json:"traces"`
			// ArticulatedTx        *SimpleFunction `json:"articulatedTx"`
		}
		tx.SetGasCost(&receipt)
		block.Transactions = append(block.Transactions, tx)
		if cache != nil {
			cache.Write(&tx, nil)
		}
	}

	if cache != nil {
		cache.Write(&block, nil)
	}
	return block, nil
}

// GetBlockByNumber fetches the block with only transactions' hashes from the RPC
func GetBlockByNumber(chain string, bn uint64, cache *cacheNew.Store) (block types.SimpleBlock[string], err error) {
	if cache != nil {
		block.BlockNumber = bn
		if err := cache.Read(&block, nil); err == nil {
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

	if cache != nil {
		cache.Write(&block, nil)
	}

	return block, nil
}

// loadBlock fetches block from RPC, but it does not try to fill Transactions field. This is delegated to
// more specialized functions and makes loadBlock generic.
func loadBlock[Tx types.BlockTransaction](chain string, bn uint64, withTxs bool) (block types.SimpleBlock[Tx], rawBlock *types.RawBlock, err error) {
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
			block.Timestamp = fmt.Sprintf("0x%x", rpc.GetBlockTimestamp(chain, 0))
		} else if mustParseUint(block.Timestamp) == 0 {
			return &types.RawBlock{}, fmt.Errorf("block at %s returned an error: %w", fmt.Sprintf("%d", bn), ethereum.NotFound)
		}

		return block, nil
	}
}
