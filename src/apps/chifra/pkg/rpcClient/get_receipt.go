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
func (conn *Connection) GetReceipt(query ReceiptQuery) (receipt types.SimpleReceipt, err error) {
	if conn.HasStore() {
		tx := &types.SimpleTransaction{
			BlockNumber:      query.Bn,
			TransactionIndex: query.Txid,
		}
		if err := conn.Store.Read(tx, nil); err == nil {
			// success
			if tx.Receipt == nil {
				return receipt, nil
			}
			return *tx.Receipt, nil
		}
	}

	rawReceipt, txHash, err := conn.getReceiptRaw(query.Bn, query.Txid)
	if err != nil {
		return
	}

	if query.NeedsTs && query.Ts == 0 {
		query.Ts = conn.GetBlockTimestamp(&query.Bn)
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
			TransactionHash:  txHash,
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
		EffectiveGasPrice: utils.MustParseUint(rawReceipt.EffectiveGasPrice),
		GasUsed:           utils.MustParseUint(rawReceipt.GasUsed),
		Logs:              logs,
		Status:            uint32(utils.MustParseUint(rawReceipt.Status)),
		IsError:           utils.MustParseUint(rawReceipt.Status) == 0,
		TransactionHash:   base.HexToHash(rawReceipt.TransactionHash),
		TransactionIndex:  utils.MustParseUint(rawReceipt.TransactionIndex),
	}
	receipt.SetRaw(rawReceipt)

	return
}

// getReceiptRaw fetches raw transaction given blockNumber and transactionIndex
func (conn *Connection) getReceiptRaw(bn uint64, txid uint64) (receipt *types.RawReceipt, hash base.Hash, err error) {
	if txHash, err := conn.GetTransactionHashByNumberAndID(bn, txid); err != nil {
		return nil, base.Hash{}, err

	} else {
		method := "eth_getTransactionReceipt"
		params := rpc.Params{txHash}
		if receipt, err := rpc.Query[types.RawReceipt](conn.Chain, method, params); err != nil {
			return nil, base.Hash{}, err
		} else {
			return receipt, txHash, nil
		}
	}
}
