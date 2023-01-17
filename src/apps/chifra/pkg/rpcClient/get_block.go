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

func GetBlockHeaderByNumber(chain string, bn uint64) (types.SimpleBlock[string], error) {
	return GetBlockByNumber(chain, bn, false)
}

// GetBlockByNumberWithTxs fetches the block with transactions from the RPC.
func GetBlockByNumberWithTxs(chain string, bn uint64, isFinal bool) (types.SimpleBlock[types.SimpleTransaction], error) {
	// load from cache if possible
	// FIXME: without updating the block in cache (writing) some value are not
	// FIXME: filled in and the tests fail
	// cached, _ := cache.GetBlock(chain, bn)
	// if cached != nil {
	// 	block = *cached
	// 	return
	// }

	block, rawBlock, err := loadBlock[types.SimpleTransaction](chain, bn, isFinal, true)
	block.SetRaw(rawBlock) // may have failed, but it's ok
	if err != nil {
		return block, err
	}

	timestamp, _ := strconv.ParseInt(rawBlock.Timestamp, 0, 64)
	block.Transactions = make([]types.SimpleTransaction, 0, len(rawBlock.Transactions))
	for _, rawTx := range rawBlock.Transactions {
		// cast transaction to a concrete type
		t, ok := rawTx.(map[string]any)
		if !ok {
			err = errors.New("cannot cast raw block transaction into map")
			return block, err
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
			return block, err
		}

		block.Transactions = append(block.Transactions, types.SimpleTransaction{
			Hash:                 txHash,
			BlockHash:            common.HexToHash(fmt.Sprint(t["blockHash"])),
			BlockNumber:          mustParseUint(t["blockNumber"]),
			TransactionIndex:     mustParseUint(t["transactionIndex"]),
			Nonce:                mustParseUint(t["nonce"]),
			Timestamp:            timestamp,
			From:                 common.HexToAddress(fmt.Sprint(t["from"])),
			To:                   common.HexToAddress(fmt.Sprint(t["to"])),
			Value:                *value,
			Gas:                  mustParseUint(t["gas"]),
			GasPrice:             txGasPrice,
			GasUsed:              receipt.GasUsed,
			MaxFeePerGas:         mustParseUint(t["maxFeePerGas"]),
			MaxPriorityFeePerGas: mustParseUint(t["maxPriorityFeePerGas"]),
			Input:                input,
			IsError:              receipt.IsError,
			HasToken:             hasToken,
			Receipt:              &receipt,
			// ExtraValue1          Wei
			// ExtraValue2          Wei
			// Cachebits            uint8           `json:"cachebits"`
			// Reserved2            uint8           `json:"reserved2"`
			// Traces               []SimpleTrace   `json:"traces"`
			// ArticulatedTx        *SimpleFunction `json:"articulatedTx"`
		})
	}

	return block, nil
}

// GetBlockByNumber fetches the block with only transactions' hashes from the RPC
func GetBlockByNumber(chain string, bn uint64, isFinal bool) (types.SimpleBlock[string], error) {
	block, rawBlock, err := loadBlock[string](chain, bn, isFinal, false)
	block.SetRaw(rawBlock) // may have failed, but it's ok
	if err != nil {
		return block, err
	}

	block.Transactions = make([]string, 0, len(rawBlock.Transactions))
	for _, rawTx := range rawBlock.Transactions {
		block.Transactions = append(block.Transactions, fmt.Sprint(rawTx))
	}

	return block, nil
}

// loadBlock fetches block from RPC, but it does not try to fill Transactions field. This is delegated to
// more specialized functions and makes loadBlock generic.
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

// ---------------------------------------------------------------------------------------------
// TODO: move this to ABI package when we port ABIs
var tokenRelated = map[string]bool{
	// functions
	"0x095ea7b3": true, // approve(address spender, uint256 value)
	"0xa9059cbb": true, // transfer(address from, uint256 to);
	"0x23b872dd": true, // transferFrom(address from, address to, uint256 value)
	"0xb3e1c718": true, // _safeMint(address, uint256)
	"0x6a4f832b": true, // _safeMint(address, uint256, bytes)
	"0xa1448194": true, // safeMint(address, uint256)
	"0x8832e6e3": true, // safeMint(address, uint256, bytes)
	"0x4e6ec247": true, // _mint(address, uint256)
	"0x4cd4edcb": true, // _mint(address, uint256, bytes, bytes)
	"0x40c10f19": true, // mint(address, uint256)
	"0xcfa84fc1": true, // mint(uint256, address[], uint256[])
	"0x278d9c41": true, // mintEventToManyUsers(uint256, address[])
	"0x78b27221": true, // mintFungible(uint256, address[], uint256[])
	"0xf9419088": true, // mintNonFungible(uint256, address[])
	"0xf190ac5f": true, // mintToAddresses(address[], uint256)
	"0xa140ae23": true, // mintToken(uint256, address)
	"0xf980f3dc": true, // mintUserToManyEvents(uint256[], address)
	"0x14004ef3": true, // multimint(address[], uint256[])
	"0x6a627842": true, // mint(address)
	"0xa0712d68": true, // mint(uint256)
	// topics
	"0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef": true, // Transfer(address from, address to, uint256 value)
	"0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925": true, // Approval(address owner, address spender, uint256 value)
	"0xd4735d920b0f87494915f556dd9b54c8f309026070caea5c737245152564d266": true, // Transfer(bytes32 node, address owner)
	"0x30385c845b448a36257a6a1716e6ad2e1bc2cbe333cde1e69fe849ad6511adfe": true, // Minted(address,uint256)
}
