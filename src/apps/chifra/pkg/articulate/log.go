package articulate

import (
	"encoding/hex"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func ArticulateLog(log *types.SimpleLog, abiMap abi.AbiInterfaceMap) (articulated *types.SimpleFunction) {
	if articulated = ParseTransferEvent(log); articulated != nil {
		return
	}
	if articulated = ParseEnsTransferEvent(log); articulated != nil {
		return
	}
	if articulated = ParseApprovalEvent(log); articulated != nil {
		return
	}

	selector := hex.EncodeToString(log.Topics[0].Bytes())
	articulated = abiMap[selector]

	return
}
