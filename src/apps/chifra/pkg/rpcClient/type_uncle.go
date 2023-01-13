package rpcClient

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

func GetUncleCountByNumber(chain string, bn uint64) (uint64, error) {
	if bn == 0 {
		return 0, nil
	}

	var response struct {
		Result string `json:"result"`
	}

	payload := RPCPayload{
		Method:    "eth_getUncleCountByBlockNumber",
		RPCParams: RPCParams{fmt.Sprintf("0x%x", bn)},
	}

	err := FromRpc(config.GetRpcProvider(chain), &payload, &response)
	if err != nil {
		return utils.NOPOS, err
	}

	val, _ := hexutil.DecodeUint64(response.Result)
	return val, err
}
