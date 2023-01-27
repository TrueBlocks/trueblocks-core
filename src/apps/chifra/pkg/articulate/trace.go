package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func ArticulateTrace(trace *types.SimpleTrace, abiMap abi.AbiInterfaceMap) (articulated *types.SimpleFunction) {
	encoding := trace.Action.Input[:10]
	return abiMap[encoding]
}
