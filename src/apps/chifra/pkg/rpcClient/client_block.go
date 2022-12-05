package rpcClient

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func GetBlockHeaderByNumber(chain string, bn uint64) (types.SimpleBlock, error) {
	return GetBlockByNumber(chain, bn, false)
}

// func GetBlockByNumber(chain string, bn uint64, withTxs bool) (types.SimpleBlock, error) {
// 	var block BlockHeader
// 	var payload = RPCPayload{
// 		Method:    "eth_getBlockByNumber",
// 		RPCParams: RPCParams{fmt.Sprintf("0x%x", bn), withTxs},
// 	}
// 	rpcProvider := config.GetRpcProvider(chain)
// 	err := FromRpc(rpcProvider, &payload, &block)
// 	if err != nil {
// 		return types.SimpleBlock{}, err
// 	}
// 	if len(block.Result.Number) == 0 || len(block.Result.Timestamp) == 0 {
// 		msg := fmt.Sprintf("block number or timestamp for %d not found", bn)
// 		return types.SimpleBlock{}, fmt.Errorf(msg)
// 	}
// 	n, _ := strconv.ParseUint(block.Result.Number[2:], 16, 64)
// 	ts, _ := strconv.ParseUint(block.Result.Timestamp[2:], 16, 64)
// 	gl, _ := strconv.ParseUint(block.Result.GasLimit[2:], 16, 64)
// 	gu, _ := strconv.ParseUint(block.Result.GasUsed[2:], 16, 64)
// 	d, _ := strconv.ParseUint(block.Result.Difficulty[2:], 16, 64)
// 	if n == 0 {
// 		ts, err = GetBlockZeroTs(chain)
// 		if err != nil {
// 			return types.SimpleBlock{}, err
// 		}
// 	}
// 	return types.SimpleBlock{
// 		BlockNumber: n,
// 		Timestamp:   int64(ts),
// 		Hash:        common.HexToHash(block.Result.Hash),
// 		ParentHash:  common.HexToHash(block.Result.ParentHash),
// 		GasLimit:    gl,
// 		GasUsed:     gu,
// 		Miner:       common.HexToAddress(block.Result.Miner),
// 		Difficulty:  d,
// 	}, nil
// }
