package rpcClient

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// GetTraceCountByNumber returns the number of traces in a block
func GetTraceCountByNumber(chain string, bn uint64) (uint64, error) {
	if traces, err := GetTracesByNumber(chain, bn); err != nil {
		return utils.NOPOS, err
	} else {
		return uint64(len(traces)), nil
	}
}

// GetTracesByNumber returns a slice of traces in a block
func GetTracesByNumber(chain string, bn uint64) ([]types.SimpleTrace, error) {
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
