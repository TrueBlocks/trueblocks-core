package rpcClient

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func GetLogCountByNumber(chain string, bn uint64) (uint64, error) {
	if bn == 0 {
		return 0, nil
	}
	var logs Logs
	logsPayload := rpc.Payload{
		Method: "eth_getLogs",
		Params: rpc.Params{LogFilter{Fromblock: fmt.Sprintf("0x%x", bn), Toblock: fmt.Sprintf("0x%x", bn)}},
	}
	err := rpc.FromRpc(config.GetRpcProvider(chain), &logsPayload, &logs)
	if err != nil {
		return utils.NOPOS, err
	}
	return uint64(len(logs.Result)), nil
}
