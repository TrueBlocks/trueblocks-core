package rpcClient

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func GetLogsByBlockNumber(chain string, bn uint64) ([]types.SimpleLog, error) {
	method := "eth_getLogs"
	params := rpc.Params{LogFilter{Fromblock: fmt.Sprintf("0x%x", bn), Toblock: fmt.Sprintf("0x%x", bn)}}

	if result, err := rpc.QuerySlice[types.SimpleLog](chain, method, params); err != nil {
		return []types.SimpleLog{}, err
	} else {
		return result, nil
	}
}

func GetLogCountByBlockNumber(chain string, bn uint64) (uint64, error) {
	if logs, err := GetLogsByBlockNumber(chain, bn); err != nil {
		return 0, err
	} else {
		return uint64(len(logs)), nil
	}
}
