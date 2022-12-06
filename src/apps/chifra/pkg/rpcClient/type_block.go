package rpcClient

import (
	"fmt"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common"
)

func GetBlockHeaderByNumber(chain string, bn uint64) (types.SimpleBlockHeader, error) {
	var blockHeader BlockHeaderResponse
	var payload = RPCPayload{
		Method:    "eth_getBlockByNumber",
		RPCParams: RPCParams{fmt.Sprintf("0x%x", bn), false},
	}
	rpcProvider := config.GetRpcProvider(chain)
	err := FromRpc(rpcProvider, &payload, &blockHeader)
	if err != nil {
		return types.SimpleBlockHeader{}, err
	}
	if len(blockHeader.Result.Number) == 0 || len(blockHeader.Result.Timestamp) == 0 {
		msg := fmt.Sprintf("block number or timestamp for %d not found", bn)
		return types.SimpleBlockHeader{}, fmt.Errorf(msg)
	}

	rawBlock := blockHeader.Result

	n, _ := strconv.ParseUint(rawBlock.Number[2:], 16, 64)
	ts, _ := strconv.ParseUint(rawBlock.Timestamp[2:], 16, 64)
	if n == 0 {
		ts, err = GetBlockZeroTs(chain)
		if err != nil {
			return types.SimpleBlockHeader{}, err
		}
	}

	block := types.SimpleBlockHeader{
		BlockNumber: n,
		Timestamp:   int64(ts),
	}

	return block, nil
}

func GetBlockByNumber(chain string, bn uint64, withTxs bool) (types.SimpleBlock, error) {
	var block BlockHeaderResponse
	var payload = RPCPayload{
		Method:    "eth_getBlockByNumber",
		RPCParams: RPCParams{fmt.Sprintf("0x%x", bn), withTxs},
	}
	rpcProvider := config.GetRpcProvider(chain)
	err := FromRpc(rpcProvider, &payload, &block)
	if err != nil {
		return types.SimpleBlock{}, err
	}
	if len(block.Result.Number) == 0 || len(block.Result.Timestamp) == 0 {
		msg := fmt.Sprintf("block number or timestamp for %d not found", bn)
		return types.SimpleBlock{}, fmt.Errorf(msg)
	}
	n, _ := strconv.ParseUint(block.Result.Number[2:], 16, 64)
	ts, _ := strconv.ParseUint(block.Result.Timestamp[2:], 16, 64)
	gl, _ := strconv.ParseUint(block.Result.GasLimit[2:], 16, 64)
	gu, _ := strconv.ParseUint(block.Result.GasUsed[2:], 16, 64)
	d, _ := strconv.ParseUint(block.Result.Difficulty[2:], 16, 64)
	if n == 0 {
		ts, err = GetBlockZeroTs(chain)
		if err != nil {
			return types.SimpleBlock{}, err
		}
	}
	return types.SimpleBlock{
		BlockNumber: n,
		Timestamp:   int64(ts),
		Hash:        common.HexToHash(block.Result.Hash),
		ParentHash:  common.HexToHash(block.Result.ParentHash),
		GasLimit:    gl,
		GasUsed:     gu,
		Miner:       common.HexToAddress(block.Result.Miner),
		Difficulty:  d,
	}, nil
}

// BlockHeaderResponse carries values returned by the `eth_getBlockByNumber` RPC command
type BlockHeaderResponse struct {
	Jsonrpc string         `json:"jsonrpc"`
	Result  types.RawBlock `json:"result"`
	ID      int            `json:"id"`
}
