package rpcClient

import (
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

func GetBlockHeaderByNumber(chain string, bn uint64) (types.SimpleBlockHeader, error) {
	rawBlock, err := getRawBlock(chain, bn, false)
	if err != nil {
		return types.SimpleBlockHeader{}, err
	}

	if len(rawBlock.Number) == 0 || len(rawBlock.Timestamp) == 0 {
		msg := fmt.Sprintf("block number or timestamp for %d not found", bn)
		return types.SimpleBlockHeader{}, fmt.Errorf(msg)
	}

	timestamp, err := hexutil.DecodeUint64(rawBlock.Timestamp)
	if err != nil {
		return types.SimpleBlockHeader{}, err
	}

	blockNumber, err := hexutil.DecodeUint64(rawBlock.Number)
	if err != nil {
		return types.SimpleBlockHeader{}, err
	}

	if blockNumber == 0 {
		timestamp, err = GetBlockZeroTs(chain)
		if err != nil {
			return types.SimpleBlockHeader{}, err
		}
	}

	block := types.SimpleBlockHeader{
		BlockNumber: blockNumber,
		Timestamp:   int64(timestamp),
	}
	block.SetRaw(*rawBlock)

	return block, nil
}

func GetBlockByNumber(chain string, bn uint64, withTxs bool) (types.SimpleBlock, error) {
	rawBlock, err := getRawBlock(chain, bn, withTxs)
	if err != nil {
		return types.SimpleBlock{}, err
	}

	timestamp, err := hexutil.DecodeUint64(rawBlock.Timestamp)
	if err != nil {
		return types.SimpleBlock{}, err
	}

	blockNumber, err := hexutil.DecodeUint64(rawBlock.Number)
	if err != nil {
		return types.SimpleBlock{}, err
	}

	gasLimit, err := hexutil.DecodeUint64(rawBlock.GasLimit)
	if err != nil {
		return types.SimpleBlock{}, err
	}

	gasUsed, err := hexutil.DecodeUint64(rawBlock.GasUsed)
	if err != nil {
		return types.SimpleBlock{}, err
	}

	difficulty, err := hexutil.DecodeUint64(rawBlock.Difficulty)
	if err != nil {
		return types.SimpleBlock{}, err
	}

	block := types.SimpleBlock{
		BlockNumber: blockNumber,
		Timestamp:   int64(timestamp),
		Hash:        common.HexToHash(rawBlock.Hash),
		ParentHash:  common.HexToHash(rawBlock.ParentHash),
		GasLimit:    gasLimit,
		GasUsed:     gasUsed,
		Miner:       common.HexToAddress(rawBlock.Miner),
		Difficulty:  difficulty,
	}
	block.SetRaw(*rawBlock)

	return block, nil
}

func getRawBlock(chain string, bn uint64, withTxs bool) (*types.RawBlock, error) {
	var response struct {
		Result types.RawBlock `json:"result"`
	}

	payload := RPCPayload{
		Method:    "eth_getBlockByNumber",
		RPCParams: RPCParams{fmt.Sprintf("0x%x", bn), withTxs},
	}

	err := FromRpc(config.GetRpcProvider(chain), &payload, &response)
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
