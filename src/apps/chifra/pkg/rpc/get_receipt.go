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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// GetReceipt retrieves a single receipt by block number and transaction id. If suggested is provided,
// it will be used for the timestamp of the logs.
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

	return rawReceipt.RawToSimple(map[string]any{
		"hash":      txHash,
		"timestmap": utils.NOPOSI,
	})
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

// GetReceiptsByNumber returns all receipts in a blocks along with their logs
func (conn *Connection) GetReceiptsByNumber(bn base.Blknum, ts base.Timestamp) ([]types.SimpleReceipt, error) {
	if conn.StoreReadable() {
		receiptGroup := &types.SimpleReceiptGroup{
			BlockNumber:      bn,
			TransactionIndex: utils.NOPOS,
		}
		if err := conn.Store.Read(receiptGroup, nil); err == nil {
			return receiptGroup.Receipts, nil
		}
	}

	if receipts, err := conn.getReceiptsSimple(bn); err != nil {
		return receipts, err
	} else {
		if conn.StoreWritable() && conn.EnabledMap["receipts"] && base.IsFinal(conn.LatestBlockTimestamp, ts) {
			receiptGroup := &types.SimpleReceiptGroup{
				Receipts:         receipts,
				BlockNumber:      bn,
				TransactionIndex: utils.NOPOS,
			}
			if err = conn.Store.Write(receiptGroup, nil); err != nil {
				logger.Warn("Failed to write receipts to cache", err)
			}
		}

		return receipts, err
	}
}

// getReceiptsSimple fetches receipts from the RPC using eth_getBlockReceipts. It returns
// an array of SimpleReceipts with the timestamp set to the block timestamp.
func (conn *Connection) getReceiptsSimple(bn base.Blknum) ([]types.SimpleReceipt, error) {
	method := "eth_getBlockReceipts"
	params := query.Params{fmt.Sprintf("0x%x", bn)}

	if rawReceipts, err := query.QuerySlice[types.RawReceipt](conn.Chain, method, params); err != nil {
		return []types.SimpleReceipt{}, err

	} else if len(rawReceipts) == 0 {
		return []types.SimpleReceipt{}, nil

	} else {
		curBlock := utils.NOPOS
		curTs := utils.NOPOSI
		var ret []types.SimpleReceipt
		for _, rawReceipt := range rawReceipts {
			rawReceipt := rawReceipt
			bn := utils.MustParseUint(rawReceipt.BlockNumber)
			if bn != curBlock {
				curTs = conn.GetBlockTimestamp(bn)
				curBlock = bn
			}
			if simp, err := rawReceipt.RawToSimple(map[string]any{
				"hash":      base.Hash{},
				"timestamp": curTs,
			}); err != nil {
				return ret, err
			} else {
				ret = append(ret, simp)
			}
		}
		return ret, nil
	}
}
