// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
	ethTypes "github.com/ethereum/go-ethereum/core/types"
)

// GetTransactionReceipt fetches receipt from the RPC. If txGasPrice is provided, it will be used for
// receipts in blocks before London
func GetTransactionReceipt(chain string, bn uint64, txid uint64, txHash *common.Hash, txGasPrice uint64) (receipt types.SimpleReceipt, err error) {
	// First, get raw receipt directly from RPC
	rawReceipt, tx, err := getRawTransactionReceipt(chain, bn, txid, txHash)
	if err != nil {
		return
	}

	// Prepare logs of type []SimpleLog
	logs := []types.SimpleLog{}
	for _, rawLog := range rawReceipt.Logs {
		rawLog := rawLog
		logAddress := base.HexToAddress(rawLog.Address)
		logIndex, parseErr := hexutil.DecodeUint64(rawLog.LogIndex)
		if parseErr != nil {
			err = parseErr
			return
		}

		logBlockNumber, parseErr := hexutil.DecodeUint64(rawLog.BlockNumber)
		if parseErr != nil {
			err = parseErr
			return
		}

		logTxIndex, parseErr := hexutil.DecodeUint64(rawLog.TransactionIndex)
		if parseErr != nil {
			err = parseErr
			return
		}

		logTopics := make([]common.Hash, 0, len(rawLog.Topics))
		for _, topic := range rawLog.Topics {
			logTopics = append(logTopics, common.HexToHash(topic))
		}

		log := types.SimpleLog{
			Address:          logAddress,
			LogIndex:         logIndex,
			BlockNumber:      logBlockNumber,
			TransactionIndex: uint32(logTxIndex),
			TransactionHash:  tx.Hash(),
			Timestamp:        0, // TODO: FIXME #2695
			Topics:           logTopics,
			Data:             string(rawLog.Data),
			CompressedLog:    "", // TODO: FIXME -- Note: we do this when displaying
		}
		log.SetRaw(&rawLog)
		logs = append(logs, log)
	}

	// Type-cast values that are not strings
	blockNumber, err := hexutil.DecodeUint64(rawReceipt.BlockNumber)
	if err != nil {
		return
	}
	cumulativeGasUsed, err := hexutil.DecodeUint64(rawReceipt.CumulativeGasUsed)
	if err != nil {
		return
	}
	gasUsed, err := hexutil.DecodeUint64(rawReceipt.GasUsed)
	if err != nil {
		return
	}
	status, err := hexutil.DecodeUint64(rawReceipt.Status)
	if err != nil {
		return
	}
	transactionIndex, err := hexutil.DecodeUint64(rawReceipt.TransactionIndex)
	if err != nil {
		return
	}

	receipt = types.SimpleReceipt{
		BlockHash:         common.HexToHash(rawReceipt.BlockHash),
		BlockNumber:       blockNumber,
		ContractAddress:   base.HexToAddress(rawReceipt.ContractAddress),
		CumulativeGasUsed: fmt.Sprint(cumulativeGasUsed),
		GasUsed:           gasUsed,
		Logs:              logs,
		Status:            uint32(status),
		IsError:           status == 0,
		TransactionHash:   common.HexToHash(rawReceipt.TransactionHash),
		TransactionIndex:  transactionIndex,
		// From:
		// To:
		// EffectiveGasPrice
	}
	receipt.SetRaw(rawReceipt)

	// TODO: this should not be hardcoded here. We have tslib.GetSpecials(), but there
	// TODO: are 2 issues with it: 1. circular dependency with types package, 2. every
	// TODO: call to GetSpecials parses CSV file, so we need to call it once and cache
	londonBlock := uint64(12965000)
	// TODO: chain specific
	if tx != nil && chain == "mainnet" {
		if blockNumber < londonBlock {
			gasPrice := txGasPrice
			if gasPrice == 0 {
				bn := tx.GasPrice()
				if bn != nil {
					gasPrice = bn.Uint64()
				}
			}
			receipt.EffectiveGasPrice = gasPrice
		}
	}
	return
}

// getRawTransactionReceipt fetches raw transaction. If txHash is provided, the function
// will not fetch the transaction (we may have already loaded it)
func getRawTransactionReceipt(chain string, bn uint64, txid uint64, txHash *common.Hash) (receipt *types.RawReceipt, tx *ethTypes.Transaction, err error) {
	fetchedTx, ferr := TxFromNumberAndId(chain, bn, txid)
	tx = &fetchedTx
	if ferr != nil {
		return nil, nil, ferr
	}
	txHashString := tx.Hash().Hex()

	var response struct {
		Result types.RawReceipt `json:"result"`
	}
	// TODO: Could use the Query stuff here
	err = rpc.FromRpc(
		config.GetRpcProvider(chain),
		&rpc.Payload{
			Method: "eth_getTransactionReceipt",
			Params: rpc.Params{txHashString},
		},
		&response,
	)
	if err != nil {
		return
	}
	receipt = &response.Result

	return
}
