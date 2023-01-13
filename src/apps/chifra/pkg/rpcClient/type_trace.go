package rpcClient

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func GetTracesByNumber(chain string, bn uint64) (traces Traces, err error) {
	if bn == 0 {
		return
	}

	tracePayload := RPCPayload{
		Method:    "trace_block",
		RPCParams: RPCParams{fmt.Sprintf("0x%x", bn)},
	}
	err = FromRpc(config.GetRpcProvider(chain), &tracePayload, &traces)
	if err != nil {
		return
	}

	return traces, nil
}

func GetTraceCountByNumber(chain string, bn uint64) (uint64, error) {
	if traces, err := GetTracesByNumber(chain, bn); err != nil {
		return utils.NOPOS, err
	} else {
		return uint64(len(traces.Result)), nil
	}
}
