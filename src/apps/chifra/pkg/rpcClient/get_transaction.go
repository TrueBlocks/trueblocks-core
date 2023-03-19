package rpcClient

import (
	"fmt"
	"math/big"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common"
)

func getRawTransaction(chain string, blockHash common.Hash, index uint64) (raw *types.RawTransaction, err error) {
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

	raw = &types.RawTransaction{
		BlockHash:        response.Result.BlockHash,
		BlockNumber:      response.Result.BlockNumber,
		ChainId:          response.Result.ChainId,
		From:             response.Result.From,
		Gas:              response.Result.Gas,
		GasPrice:         response.Result.GasPrice,
		Hash:             response.Result.Hash,
		Input:            response.Result.Input,
		Nonce:            response.Result.Nonce,
		R:                response.Result.R,
		S:                response.Result.S,
		To:               response.Result.To,
		TransactionIndex: response.Result.TransactionIndex,
		Type:             response.Result.Type,
		V:                response.Result.V,
		Value:            response.Result.Value,
	}

	return
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
	txHash := common.HexToHash(rawTx.Hash)
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
		BlockHash:        common.HexToHash(rawTx.BlockHash),
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
