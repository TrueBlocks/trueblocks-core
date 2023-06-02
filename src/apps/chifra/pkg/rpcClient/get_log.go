package rpcClient

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/bykof/gostradamus"
)

func GetLogsByBlockNumber(chain string, bn uint64) ([]types.SimpleLog, error) {
	method := "eth_getLogs"
	params := rpc.Params{LogFilter{Fromblock: fmt.Sprintf("0x%x", bn), Toblock: fmt.Sprintf("0x%x", bn)}}

	ts := rpc.GetBlockTimestamp(chain, 0)
	date := gostradamus.FromUnixTimestamp(ts)
	if rawLogs, err := rpc.QuerySlice[types.RawLog](chain, method, params); err != nil {
		return []types.SimpleLog{}, err
	} else {
		var ret []types.SimpleLog
		for _, rawLog := range rawLogs {
			log := types.SimpleLog{
				Address:          base.HexToAddress(rawLog.Address),
				BlockHash:        base.HexToHash(rawLog.BlockHash),
				BlockNumber:      mustParseUint(rawLog.BlockNumber),
				Data:             rawLog.Data,
				LogIndex:         mustParseUint(rawLog.LogIndex),
				Timestamp:        ts,
				Date:             date.String(),
				TransactionHash:  base.HexToHash(rawLog.TransactionHash),
				TransactionIndex: mustParseUint(rawLog.TransactionIndex),
			}
			for _, topic := range rawLog.Topics {
				log.Topics = append(log.Topics, base.HexToHash(topic))
			}
			log.SetRaw(&rawLog)
			ret = append(ret, log)
		}
		return ret, nil
	}
}

func GetLogCountByBlockNumber(chain string, bn uint64) (uint64, error) {
	if logs, err := GetLogsByBlockNumber(chain, bn); err != nil {
		return 0, err
	} else {
		return uint64(len(logs)), nil
	}
}
