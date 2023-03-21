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
	"github.com/ethereum/go-ethereum/common/hexutil"
	ethTypes "github.com/ethereum/go-ethereum/core/types"
)

// GetTransactionReceipt fetches receipt from the RPC. If txGasPrice is provided, it will be used for
// receipts in blocks before London
func GetTransactionReceipt(chain string, bn uint64, txid uint64, txHash *base.Hash, txGasPrice uint64) (receipt types.SimpleReceipt, err error) {
	// First, get raw receipt directly from RPC
	rawReceipt, tx, err := getRawTransactionReceipt(chain, bn, txid, txHash)
	if err != nil {
		return
	}

	// Prepare logs of type []SimpleLog
	logs := []types.SimpleLog{}
	for _, rawLog := range rawReceipt.Logs {
		rawLog := rawLog

		log := types.SimpleLog{
			Address:          base.HexToAddress(rawLog.Address),
			LogIndex:         mustParseUint(rawLog.LogIndex),
			BlockNumber:      mustParseUint(rawLog.BlockNumber),
			BlockHash:        base.HexToHash(rawLog.BlockHash),
			TransactionIndex: mustParseUint(rawLog.TransactionIndex),
			TransactionHash:  base.HexToHash(tx.Hash().Hex()),
			Timestamp:        0, // TODO: FIXME #2695
			Data:             string(rawLog.Data),
		}
		for _, topic := range rawLog.Topics {
			log.Topics = append(log.Topics, base.HexToHash(topic))
		}

		log.SetRaw(&rawLog)
		logs = append(logs, log)
	}

	cumulativeGasUsed, err := hexutil.DecodeUint64(rawReceipt.CumulativeGasUsed)
	if err != nil {
		return
	}

	receipt = types.SimpleReceipt{
		BlockHash:         base.HexToHash(rawReceipt.BlockHash),
		BlockNumber:       mustParseUint(rawReceipt.BlockNumber),
		ContractAddress:   base.HexToAddress(rawReceipt.ContractAddress),
		CumulativeGasUsed: fmt.Sprint(cumulativeGasUsed),
		GasUsed:           mustParseUint(rawReceipt.GasUsed),
		Logs:              logs,
		Status:            uint32(mustParseUint(rawReceipt.Status)),
		IsError:           mustParseUint(rawReceipt.Status) == 0,
		TransactionHash:   base.HexToHash(rawReceipt.TransactionHash),
		TransactionIndex:  mustParseUint(rawReceipt.TransactionIndex),
	}
	receipt.SetRaw(rawReceipt)

	// TODO: this should not be hardcoded here. We have tslib.GetSpecials(), but there
	// TODO: are 2 issues with it: 1. circular dependency with types package, 2. every
	// TODO: call to GetSpecials parses CSV file, so we need to call it once and cache
	londonBlock := uint64(12965000)
	// TODO: chain specific
	if tx != nil && chain == "mainnet" {
		if receipt.BlockNumber < londonBlock {
			gasPrice := txGasPrice
			if gasPrice == 0 {
				bn := tx.GasPrice()
				if bn != nil {
					gasPrice = bn.Uint64()
				}
			}
			// TODO: This is very likely untested simply because our test cases are early blocks
			receipt.EffectiveGasPrice = gasPrice
		}
	}
	return
}

// getRawTransactionReceipt fetches raw transaction. If txHash is provided, the function
// will not fetch the transaction (we may have already loaded it)
func getRawTransactionReceipt(chain string, bn uint64, txid uint64, txHash *base.Hash) (receipt *types.RawReceipt, tx *ethTypes.Transaction, err error) {
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
