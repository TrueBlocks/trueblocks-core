// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpc

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetReceipt retrieves a single receipt by block number and transaction id. If suggested is provided,
// it will be used for the timestamp of the logs.
func (conn *Connection) GetReceipt(bn base.Blknum, txid base.Txnum, suggested base.Timestamp) (receipt types.Receipt, err error) {
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
func (conn *Connection) GetReceiptNoTimestamp(bn base.Blknum, txid base.Txnum) (receipt types.Receipt, err error) {
	if conn.StoreReadable() {
		tx := &types.Transaction{
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

	return conn.getReceiptRaw(bn, txid)
}

// TODO: BOGUS - clean raw
// getReceiptRaw fetches transaction given blockNumber and transactionIndex
func (conn *Connection) getReceiptRaw(bn base.Blknum, txid base.Txnum) (receipt types.Receipt, err error) {
	if txHash, err := conn.GetTransactionHashByNumberAndID(bn, txid); err != nil {
		return types.Receipt{}, err

	} else {
		method := "eth_getTransactionReceipt"
		params := query.Params{txHash}

		if receipt, err := query.Query[types.Receipt](conn.Chain, method, params); err != nil {
			return types.Receipt{}, err
		} else {
			receipt.IsError = receipt.Status == 0
			return *receipt, nil
		}
	}
}

// GetReceiptsByNumber returns all receipts in a blocks along with their logs
func (conn *Connection) GetReceiptsByNumber(bn base.Blknum, ts base.Timestamp) ([]types.Receipt, map[base.Txnum]*types.Receipt, error) {
	if conn.StoreReadable() {
		receiptGroup := &types.ReceiptGroup{
			BlockNumber:      bn,
			TransactionIndex: base.NOPOSN,
		}
		if err := conn.Store.Read(receiptGroup, nil); err == nil {
			receiptMap := make(map[base.Txnum]*types.Receipt, len(receiptGroup.Receipts))
			for index := 0; index < len(receiptGroup.Receipts); index++ {
				pReceipt := &receiptGroup.Receipts[index]
				receiptMap[pReceipt.TransactionIndex] = pReceipt
			}
			return receiptGroup.Receipts, receiptMap, nil
		}
	}

	if receipts, err := conn.getReceipts(bn); err != nil {
		return receipts, nil, err
	} else {
		if conn.StoreWritable() && conn.EnabledMap["receipts"] && base.IsFinal(conn.LatestBlockTimestamp, ts) {
			receiptGroup := &types.ReceiptGroup{
				Receipts:         receipts,
				BlockNumber:      bn,
				TransactionIndex: base.NOPOSN,
			}
			if err = conn.Store.Write(receiptGroup, nil); err != nil {
				logger.Warn("Failed to write receipts to cache", err)
			}
		}

		receiptMap := make(map[base.Txnum]*types.Receipt, len(receipts))
		for index := 0; index < len(receipts); index++ {
			pReceipt := &receipts[index]
			receiptMap[pReceipt.TransactionIndex] = pReceipt
		}
		return receipts, receiptMap, err
	}
}

// getReceipts fetches receipts from the RPC using eth_getBlockReceipts. It returns
// an array of Receipts with the timestamp set to the block timestamp.
func (conn *Connection) getReceipts(bn base.Blknum) ([]types.Receipt, error) {
	method := "eth_getBlockReceipts"
	params := query.Params{fmt.Sprintf("0x%x", bn)}

	if rawReceipts, err := query.Query[[]types.Receipt](conn.Chain, method, params); err != nil {
		return []types.Receipt{}, err

	} else if rawReceipts == nil || len(*rawReceipts) == 0 {
		return []types.Receipt{}, nil

	} else {
		var ret []types.Receipt
		for _, receipt := range *rawReceipts {
			receipt.IsError = receipt.Status == 0
			ret = append(ret, receipt)
		}
		return ret, nil
	}
}
