// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpc

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

func (conn *Connection) GetReceipt(bn base.Blknum, txid base.Txnum, suggested base.Timestamp) (receipt types.SimpleReceipt, err error) {
	receipt, err = conn.GetReceiptNoTimestamp(bn, txid) // note that the logs do not yet have timestamp...

	if suggested == 0 {
		suggested = conn.GetBlockTimestamp(bn)
	}
	for index := 0; index < len(receipt.Logs); index++ {
		receipt.Logs[index].Timestamp = suggested
	}
	return receipt, err
}

// GetReceiptNoTimestamp fetches receipt from the RPC. If txGasPrice is provided, it will be used for
// receipts in blocks before London
func (conn *Connection) GetReceiptNoTimestamp(bn base.Blknum, txid base.Txnum) (receipt types.SimpleReceipt, err error) {
	if conn.StoreReadable() {
		tx := &types.SimpleTransaction{
			BlockNumber:      bn,
			TransactionIndex: txid,
		}
		if err := conn.Store.Read(tx, nil); err == nil {
			// success
			if tx.Receipt == nil {
				return receipt, nil
			}
			return *tx.Receipt, nil
		}
	}

	rawReceipt, txHash, err := conn.getReceiptRaw(bn, txid)
	if err != nil {
		return
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
		params := query.Params{txHash}
		if receipt, err := query.Query[types.RawReceipt](conn.Chain, method, params); err != nil {
			return nil, base.Hash{}, err
		} else {
			return receipt, txHash, nil
		}
	}
}
