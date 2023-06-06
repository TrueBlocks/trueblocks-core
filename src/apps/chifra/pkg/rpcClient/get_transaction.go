package rpcClient

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

var (
	notAnInt = utils.NOPOS
	notAHash = base.Hash{}
)

func getRawTransaction(chain string, blkHash base.Hash, txHash base.Hash, bn base.Blknum, txid uint64) (raw *types.RawTransaction, err error) {
	method := "eth_getTransactionByBlockNumberAndIndex"
	params := rpc.Params{fmt.Sprintf("0x%x", bn), fmt.Sprintf("0x%x", txid)}
	if txHash != notAHash {
		method = "eth_getTransactionByHash"
		params = rpc.Params{txHash.Hex()}
	} else if blkHash != notAHash {
		method = "eth_getTransactionByBlockHashAndIndex"
		params = rpc.Params{blkHash.Hex(), fmt.Sprintf("0x%x", txid)}
	}

	if trans, err := rpc.Query[types.RawTransaction](chain, method, params); err != nil {
		return &types.RawTransaction{}, err
	} else {
		if trans.AccessList == nil {
			trans.AccessList = make([]types.StorageSlot, 0)
		}
		return trans, nil
	}
}

func GetRawTransactionByHashAndId(chain string, blkHash base.Hash, txid uint64) (raw *types.RawTransaction, err error) {
	return getRawTransaction(chain, blkHash, notAHash, notAnInt, txid)
}

func GetRawTransactionByHash(chain string, txHash base.Hash) (raw *types.RawTransaction, err error) {
	return getRawTransaction(chain, notAHash, txHash, notAnInt, notAnInt)
}

func GetRawTransactionByBlockAndId(chain string, bn base.Blknum, txid uint64) (raw *types.RawTransaction, err error) {
	return getRawTransaction(chain, notAHash, notAHash, bn, txid)
}

func GetAppearanceFromHash(chain string, hash string) (uint64, uint64, error) {
	if rawTx, err := GetRawTransactionByHash(chain, base.HexToHash(hash)); err != nil {
		return 0, 0, err
	} else {
		return mustParseUint(rawTx.BlockNumber), mustParseUint(rawTx.TransactionIndex), nil
	}
}

func GetTransactionByAppearance(chain string, appearance *types.RawAppearance, fetchTraces bool) (tx *types.SimpleTransaction, err error) {
	bn := uint64(appearance.BlockNumber)
	txid := uint64(appearance.TransactionIndex)

	blockTs := rpc.GetBlockTimestamp(chain, bn)
	receipt, err := GetTransactionReceipt(chain, ReceiptQuery{
		Bn:      bn,
		Txid:    txid,
		NeedsTs: true,
		Ts:      blockTs,
	})
	if err != nil {
		return
	}

	rawTx, err := GetRawTransactionByBlockAndId(chain, bn, txid)
	if err != nil {
		return
	}

	tx = &types.SimpleTransaction{
		Hash:             base.HexToHash(rawTx.Hash),
		BlockHash:        base.HexToHash(rawTx.BlockHash),
		BlockNumber:      bn,
		TransactionIndex: txid,
		Nonce:            mustParseUint(rawTx.Nonce),
		Timestamp:        blockTs,
		From:             base.HexToAddress(rawTx.From),
		To:               base.HexToAddress(rawTx.To),
		Gas:              mustParseUint(rawTx.Gas),
		GasPrice:         mustParseUint(rawTx.GasPrice),
		GasUsed:          receipt.GasUsed,
		Input:            rawTx.Input,
		IsError:          receipt.IsError,
		HasToken:         IsTokenRelated(rawTx.Input),
		Receipt:          &receipt,
	}
	tx.Value.SetString(rawTx.Value, 0)
	tx.SetGasCost(&receipt)
	tx.SetRaw(rawTx)

	if fetchTraces {
		traces, err := GetTracesByTransactionHash(chain, tx.Hash.Hex(), tx)
		if err != nil {
			return nil, err
		}
		tx.Traces = traces
	}

	return
}
