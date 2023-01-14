package rpcClient

import (
	"fmt"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func GetTracesByNumber(chain string, bn uint64) ([]types.SimpleTrace, error) {
	mustParseUint := func(input any) (result uint64) {
		result, _ = strconv.ParseUint(fmt.Sprint(input), 0, 64)
		return
	}

	method := "trace_block"
	params := rpc.Params{fmt.Sprintf("0x%x", bn)}

	if rawTrace, err := rpc.QuerySlice[types.RawTrace](chain, method, params); err != nil {
		return []types.SimpleTrace{}, err
	} else {
		var ret []types.SimpleTrace
		for _, trace := range rawTrace {
			ret = append(ret, types.SimpleTrace{
				BlockNumber: mustParseUint(trace.BlockNumber),
			})
		}
		return ret, nil
	}
}

func GetTraceCountByNumber(chain string, bn uint64) (uint64, error) {
	if traces, err := GetTracesByNumber(chain, bn); err != nil {
		return utils.NOPOS, err
	} else {
		return uint64(len(traces)), nil
	}
}
