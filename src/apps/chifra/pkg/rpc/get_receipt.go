// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpc

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/walk"
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
	tx := &types.Transaction{
		BlockNumber:      bn,
		TransactionIndex: txid,
	}
	if err := conn.ReadFromCache(tx); err == nil {
		if tx.Receipt == nil {
			return receipt, nil
		}
		return *tx.Receipt, nil
	}

	// TODO: Bogus - weird code related to with or without timestamp. There's a better way
	return conn.getReceiptFromRpc(bn, txid)
}

// getReceiptFromRpc fetches transaction given blockNumber and transactionIndex
func (conn *Connection) getReceiptFromRpc(bn base.Blknum, txid base.Txnum) (receipt types.Receipt, err error) {
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
	receiptGroup := &types.ReceiptGroup{
		BlockNumber:      bn,
		TransactionIndex: base.NOPOSN,
	}
	if err := conn.ReadFromCache(receiptGroup); err == nil {
		receiptMap := make(map[base.Txnum]*types.Receipt, len(receiptGroup.Receipts))
		for index := 0; index < len(receiptGroup.Receipts); index++ {
			pReceipt := &receiptGroup.Receipts[index]
			receiptMap[pReceipt.TransactionIndex] = pReceipt
		}
		return receiptGroup.Receipts, receiptMap, nil
	}

	if receipts, err := conn.getBlockReceiptsFromRpc(bn); err != nil {
		return receipts, nil, err
	} else {
		receiptGroup := &types.ReceiptGroup{
			BlockNumber:      bn,
			TransactionIndex: base.NOPOSN,
			Receipts:         receipts,
		}

		receiptMap := make(map[base.Txnum]*types.Receipt, len(receipts))

		// even if there's an error, this spin through won't matter
		err = conn.WriteToCache(receiptGroup, walk.Cache_Receipts, ts)
		for index := 0; index < len(receipts); index++ {
			pReceipt := &receipts[index]
			receiptMap[pReceipt.TransactionIndex] = pReceipt
		}
		return receipts, receiptMap, err
	}
}

// getBlockReceiptsFromRpc fetches receipts from the RPC using eth_getBlockReceipts. It returns
// an array of Receipts with the timestamp set to the block timestamp.
func (conn *Connection) getBlockReceiptsFromRpc(bn base.Blknum) ([]types.Receipt, error) {
	method := "eth_getBlockReceipts"
	params := query.Params{fmt.Sprintf("0x%x", bn)}

	if receipts, err := query.Query[[]types.Receipt](conn.Chain, method, params); err != nil {
		return []types.Receipt{}, err

	} else if receipts == nil || len(*receipts) == 0 {
		return []types.Receipt{}, nil

	} else {
		var ret []types.Receipt
		for _, receipt := range *receipts {
			receipt.IsError = receipt.Status == 0
			ret = append(ret, receipt)
		}
		return ret, nil
	}
}
