package rpcClient

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func GetLogCountByNumber(chain string, bn uint64) (uint64, error) {
	if bn == 0 {
		return 0, nil
	}
	var logs Logs
	logsPayload := RPCPayload{
		Method:    "eth_getLogs",
		RPCParams: RPCParams{LogFilter{Fromblock: fmt.Sprintf("0x%x", bn), Toblock: fmt.Sprintf("0x%x", bn)}},
	}
	err := FromRpc(config.GetRpcProvider(chain), &logsPayload, &logs)
	if err != nil {
		return utils.NOPOS, err
	}
	return uint64(len(logs.Result)), nil
}
