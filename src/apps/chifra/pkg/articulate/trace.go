package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func ArticulateTrace(trace *types.SimpleTrace, abiMap abi.AbiInterfaceMap) (articulated *types.SimpleFunction, err error) {
	input := trace.Action.Input
	if len(input) < 10 {
		return
	}
	encoding := input[:10]
	articulated = abiMap[encoding]

	if trace.Result == nil || articulated == nil {
		return
	}

	err = ArticulateArguments(
		articulated.GetAbiMethod().Outputs,
		trace.Result.Output[2:],
		nil,
		articulated.Outputs,
	)

	return
}
