package rpcClient

import (
	"fmt"
	"math/big"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func getRawTransaction(chain string, blockHash base.Hash, index uint64) (raw *types.RawTransaction, err error) {
	var response struct {
		Result types.RawTransaction `json:"result"`
	}
	payload := rpc.Payload{
		Method: "eth_getTransactionByBlockHashAndIndex",
		Params: rpc.Params{
			blockHash.Hex(),
			fmt.Sprintf("0x%x", index),
		},
	}

	if err = rpc.FromRpc(config.GetRpcProvider(chain), &payload, &response); err != nil {
		return nil, err
	}

	raw = &response.Result
	return
}

// TxNumberAndIdFromHash returns a transaction's blockNum and tx_id given its hash
func TxNumberAndIdFromHash(provider string, hash string) (uint64, uint64, error) {
	var trans Transaction
	transPayload := rpc.Payload{
		Method: "eth_getTransactionByHash",
		Params: rpc.Params{hash},
	}
	// TODO: Use rpc.Query
	err := rpc.FromRpc(provider, &transPayload, &trans)
	if err != nil {
		fmt.Println("rpc.FromRpc(traces) returned error")
		logger.Fatal(err)
	}
	if trans.Result.BlockNumber == "" {
		return 0, 0, fmt.Errorf("transaction at %s reported an error: %w", hash, ethereum.NotFound)
	}
	bn, err := strconv.ParseUint(trans.Result.BlockNumber[2:], 16, 32)
	if err != nil {
		return 0, 0, err
	}
	txid, err := strconv.ParseUint(trans.Result.TransactionIndex[2:], 16, 32)
	if err != nil {
		return 0, 0, err
	}
	return bn, txid, nil
}

func GetTransactionByAppearance(chain string, appearance *types.RawAppearance, fetchTraces bool) (tx *types.SimpleTransaction, err error) {
	blockNumber := uint64(appearance.BlockNumber)
	txIndex := uint64(appearance.TransactionIndex)
	block, err := GetBlockByNumber(chain, blockNumber, false)
	if err != nil {
		return
	}
	rawTx, err := getRawTransaction(chain, block.Hash, txIndex)
	if err != nil {
		return
	}
	txHash := base.HexToHash(rawTx.Hash)
	receipt, err := GetTransactionReceipt(
		chain,
		blockNumber,
		txIndex,
		nil,
		0,
	)
	if err != nil {
		return
	}
	value := big.NewInt(0)
	value.SetString(rawTx.Value, 0)

	hasToken := false
	if len(rawTx.Input) >= 10 {
		hasToken = IsTokenRelated(rawTx.Input[:10])
	}
	txBlockNumber, err := strconv.ParseUint(rawTx.BlockNumber, 0, 64)
	if err != nil {
		return
	}
	nonce, err := strconv.ParseUint(rawTx.Nonce, 0, 64)
	if err != nil {
		return
	}
	gas, err := strconv.ParseUint(rawTx.Gas, 0, 64)
	if err != nil {
		return
	}
	gasPrice, err := strconv.ParseUint(rawTx.GasPrice, 0, 64)
	if err != nil {
		return
	}

	tx = &types.SimpleTransaction{
		Hash:             txHash,
		BlockHash:        base.HexToHash(rawTx.BlockHash),
		BlockNumber:      txBlockNumber,
		TransactionIndex: txIndex,
		Nonce:            nonce,
		Timestamp:        block.Timestamp,
		From:             base.HexToAddress(rawTx.From),
		To:               base.HexToAddress(rawTx.To),
		Value:            *value,
		Gas:              gas,
		GasPrice:         gasPrice,
		GasUsed:          receipt.GasUsed,
		// TODO:
		// MaxFeePerGas:
		// MaxPriorityFeePerGas:
		Input:    rawTx.Input,
		IsError:  receipt.IsError,
		HasToken: hasToken,
		Receipt:  &receipt,
		// Traces               []SimpleTrace   `json:"traces"`
		// ArticulatedTx        *SimpleFunction `json:"articulatedTx"`
	}
	tx.SetGasCost(&receipt)
	tx.SetRaw(rawTx)

	if fetchTraces {
		traces, err := GetTracesByTransactionHash(chain, txHash.Hex(), tx)
		if err != nil {
			return nil, err
		}
		tx.Traces = traces
	}

	return
}
