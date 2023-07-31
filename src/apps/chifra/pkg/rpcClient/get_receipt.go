// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common/hexutil"
	ethTypes "github.com/ethereum/go-ethereum/core/types"
)

type ReceiptQuery struct {
	Bn       uint64
	Txid     uint64
	TxHash   *base.Hash
	GasPrice uint64
	NeedsTs  bool
	Ts       base.Timestamp
}

// GetReceipt fetches receipt from the RPC. If txGasPrice is provided, it will be used for
// receipts in blocks before London
func (options *Options) GetReceipt(chain string, query ReceiptQuery) (receipt types.SimpleReceipt, err error) {
	if options.HasStore() {
		tx := &types.SimpleTransaction{
			BlockNumber:      query.Bn,
			TransactionIndex: query.Txid,
		}
		if err := options.Store.Read(tx, nil); err == nil {
			// success
			if tx.Receipt == nil {
				return receipt, nil
			}
			return *tx.Receipt, nil
		}
	}

	rawReceipt, tx, err := getRawTransactionReceipt(chain, query.Bn, query.Txid)
	if err != nil {
		return
	}

	if query.NeedsTs && query.Ts == 0 {
		query.Ts = options.GetBlockTimestamp(chain, &query.Bn)
	}

	logs := []types.SimpleLog{}
	for _, rawLog := range rawReceipt.Logs {
		rawLog := rawLog
		log := types.SimpleLog{
			Address:          base.HexToAddress(rawLog.Address),
			LogIndex:         utils.MustParseUint(rawLog.LogIndex),
			BlockNumber:      utils.MustParseUint(rawLog.BlockNumber),
			BlockHash:        base.HexToHash(rawLog.BlockHash),
			TransactionIndex: utils.MustParseUint(rawLog.TransactionIndex),
			TransactionHash:  base.HexToHash(tx.Hash().Hex()),
			Timestamp:        query.Ts,
			Date:             utils.FormattedDate(query.Ts),
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
		BlockNumber:       utils.MustParseUint(rawReceipt.BlockNumber),
		ContractAddress:   base.HexToAddress(rawReceipt.ContractAddress),
		CumulativeGasUsed: fmt.Sprint(cumulativeGasUsed),
		GasUsed:           utils.MustParseUint(rawReceipt.GasUsed),
		Logs:              logs,
		Status:            uint32(utils.MustParseUint(rawReceipt.Status)),
		IsError:           utils.MustParseUint(rawReceipt.Status) == 0,
		TransactionHash:   base.HexToHash(rawReceipt.TransactionHash),
		TransactionIndex:  utils.MustParseUint(rawReceipt.TransactionIndex),
	}
	receipt.SetRaw(rawReceipt)

	// TODO: this should not be hardcoded here. We have tslib.GetSpecials(), but there
	// TODO: are 2 issues with it: 1. circular dependency with types package, 2. every
	// TODO: call to GetSpecials parses CSV file, so we need to call it once and cache
	londonBlock := uint64(12965000)
	// TODO: chain specific
	if tx != nil && chain == "mainnet" {
		if receipt.BlockNumber < londonBlock {
			gasPrice := query.GasPrice
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

// getRawTransactionReceipt fetches raw transaction given blockNumber and transactionIndex
func getRawTransactionReceipt(chain string, bn uint64, txid uint64) (receipt *types.RawReceipt, tx *ethTypes.Transaction, err error) {
	rpcOptions := DefaultRpcOptions(&DefaultRpcOptionsSettings{
		Chain: chain,
	})
	if fetched, err := rpcOptions.GetTransactionByNumberAndID(chain, bn, txid); err != nil {
		return nil, nil, err

	} else {
		method := "eth_getTransactionReceipt"
		params := rpc.Params{fetched.Hash().Hex()}
		if receipt, err := rpc.Query[types.RawReceipt](chain, method, params); err != nil {
			return nil, nil, err
		} else {
			return receipt, &fetched, nil
		}
	}
}
