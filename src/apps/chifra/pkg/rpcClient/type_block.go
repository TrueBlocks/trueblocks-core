package rpcClient

import (
	"errors"
	"fmt"
	"math/big"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

func GetBlockHeaderByNumber(chain string, bn uint64) (types.SimpleBlockHeader, error) {
	rawBlock, err := getRawBlock(chain, bn, false)
	if err != nil {
		return types.SimpleBlockHeader{}, err
	}

	timestamp, err := hexutil.DecodeUint64(rawBlock.Timestamp)
	if err != nil {
		return types.SimpleBlockHeader{}, err
	}

	blockNumber, err := hexutil.DecodeUint64(rawBlock.Number)
	if err != nil {
		return types.SimpleBlockHeader{}, err
	}

	block := types.SimpleBlockHeader{
		BlockNumber: blockNumber,
		Timestamp:   int64(timestamp),
	}
	block.SetRaw(*rawBlock)

	return block, nil
}

// loadBlock fetches block from RPC, but it does not try to fill Transactions field. This is delegated to more specialized functions and makes
// loadBlock generic.
func loadBlock[Tx types.BlockTransaction](chain string, bn uint64, isFinal bool, withTxs bool) (block types.SimpleBlock[Tx], rawBlock *types.RawBlock, err error) {
	rawBlock, err = getRawBlock(chain, bn, withTxs)
	if err != nil {
		return
	}

	timestamp, err := hexutil.DecodeUint64(rawBlock.Timestamp)
	if err != nil {
		return
	}

	blockNumber, err := hexutil.DecodeUint64(rawBlock.Number)
	if err != nil {
		return
	}

	gasLimit, err := hexutil.DecodeUint64(rawBlock.GasLimit)
	if err != nil {
		return
	}

	gasUsed, err := hexutil.DecodeUint64(rawBlock.GasUsed)
	if err != nil {
		return
	}

	difficulty, err := hexutil.DecodeUint64(rawBlock.Difficulty)
	if err != nil {
		return
	}

	uncles := make([]common.Hash, 0, len(rawBlock.Uncles))
	for _, uncle := range rawBlock.Uncles {
		uncles = append(uncles, common.HexToHash(uncle))
	}

	block = types.SimpleBlock[Tx]{
		BlockNumber: blockNumber,
		Timestamp:   int64(timestamp),
		Hash:        common.HexToHash(rawBlock.Hash),
		ParentHash:  common.HexToHash(rawBlock.ParentHash),
		GasLimit:    gasLimit,
		GasUsed:     gasUsed,
		Miner:       common.HexToAddress(rawBlock.Miner),
		Difficulty:  difficulty,
		Finalized:   isFinal,
		Uncles:      uncles,
	}
	return
}

// GetBlockByNumberWithTxs fetches the block with transactions from the RPC.
func GetBlockByNumberWithTxs(chain string, bn uint64, isFinal bool) (block types.SimpleBlock[types.SimpleTransaction], err error) {
	// load from cache if possible
	// FIXME: without updating the block in cache (writing) some value are not
	// FIXME: filled in and the tests fail
	// cached, _ := cache.GetBlock(chain, bn)
	// if cached != nil {
	// 	block = *cached
	// 	return
	// }

	mustParseUint := func(input any) (result uint64) {
		result, _ = strconv.ParseUint(fmt.Sprint(input), 0, 64)
		return
	}

	var rawBlock *types.RawBlock
	block, rawBlock, err = loadBlock[types.SimpleTransaction](chain, bn, isFinal, true)
	if err != nil {
		return
	}
	block.SetRaw(*rawBlock)

	timestamp, _ := strconv.ParseInt(rawBlock.Timestamp, 0, 64)
	block.Transactions = make([]types.SimpleTransaction, 0, len(rawBlock.Transactions))
	for _, rawTx := range rawBlock.Transactions {
		// cast transaction to a concrete type
		t, ok := rawTx.(map[string]any)
		if !ok {
			err = errors.New("cannot cast raw block transaction into map")
			return
		}

		txHash := common.HexToHash(fmt.Sprint(t["hash"]))
		txGasPrice := mustParseUint(t["gasPrice"])
		input := fmt.Sprint(t["input"])
		value := big.NewInt(0)
		value.SetString(fmt.Sprint(t["value"]), 0)

		var hasToken bool
		if len(input) >= 10 {
			hasToken = tokenRelated[input[0:9]]
		}

		// Get the receipt
		var receipt types.SimpleReceipt
		receipt, err = GetTransactionReceipt(chain, bn, 0, &txHash, txGasPrice)
		if err != nil {
			return
		}

		block.Transactions = append(block.Transactions, types.SimpleTransaction{
			Hash:             txHash,
			BlockHash:        common.HexToHash(fmt.Sprint(t["blockHash"])),
			BlockNumber:      mustParseUint(t["blockNumber"]),
			TransactionIndex: mustParseUint(t["transactionIndex"]),
			Nonce:            mustParseUint(t["nonce"]),
			Timestamp:        timestamp,
			From:             common.HexToAddress(fmt.Sprint(t["from"])),
			To:               common.HexToAddress(fmt.Sprint(t["to"])),
			Value:            *value,
			// ExtraValue1          Wei
			// ExtraValue2          Wei
			Gas:                  mustParseUint(t["gas"]),
			GasPrice:             txGasPrice,
			GasUsed:              receipt.GasUsed,
			MaxFeePerGas:         mustParseUint(t["maxFeePerGas"]),
			MaxPriorityFeePerGas: mustParseUint(t["maxPriorityFeePerGas"]),
			Input:                input,
			IsError:              receipt.IsError,
			HasToken:             hasToken,
			// Cachebits            uint8           `json:"cachebits"`
			// Reserved2            uint8           `json:"reserved2"`
			Receipt: &receipt,
			// Traces               []SimpleTrace   `json:"traces"`
			// ArticulatedTx        *SimpleFunction `json:"articulatedTx"`
		})
	}

	return block, nil
}

// GetBlockByNumber fetches the block with only transactions' hashes from the RPC
func GetBlockByNumber(chain string, bn uint64, isFinal bool) (block types.SimpleBlock[string], err error) {
	var rawBlock *types.RawBlock
	block, rawBlock, err = loadBlock[string](chain, bn, isFinal, false)
	if err != nil {
		return
	}
	block.SetRaw(*rawBlock)

	block.Transactions = make([]string, 0, len(rawBlock.Transactions))
	for _, rawTx := range rawBlock.Transactions {
		s := fmt.Sprint(rawTx)
		block.Transactions = append(block.Transactions, s)
	}

	return block, nil
}

func getRawBlock(chain string, bn uint64, withTxs bool) (*types.RawBlock, error) {
	var response struct {
		Result types.RawBlock `json:"result"`
	}

	payload := rpc.Payload{
		Method: "eth_getBlockByNumber",
		Params: rpc.Params{fmt.Sprintf("0x%x", bn), withTxs},
	}

	err := rpc.FromRpc(config.GetRpcProvider(chain), &payload, &response)
	if err != nil {
		return &types.RawBlock{}, err
	}

	if bn == 0 {
		// The RPC does not return a timestamp for the zero block, so we make one
		var ts uint64
		ts, err = GetBlockZeroTs(chain)
		if err != nil {
			return &types.RawBlock{}, err
		}
		response.Result.Timestamp = fmt.Sprintf("0x%x", ts)
	}

	rawBlock := &response.Result
	if len(response.Result.Timestamp) == 0 {
		msg := fmt.Sprintf("block number or timestamp for %d not found", bn)
		return rawBlock, errors.New(msg)
	}

	return rawBlock, err
}
