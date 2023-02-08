package articulate

import (
	"encoding/hex"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func ArticulateLog(log *types.SimpleLog, abiMap abi.AbiInterfaceMap) (articulated *types.SimpleFunction, err error) {
	if articulated = ParseTransferEvent(log); articulated != nil {
		return
	}
	if articulated = ParseEnsTransferEvent(log); articulated != nil {
		return
	}
	if articulated = ParseApprovalEvent(log); articulated != nil {
		return
	}

	selector := "0x" + hex.EncodeToString(log.Topics[0].Bytes())
	articulated = abiMap[selector]

	if articulated == nil {
		return
	}

	err = ArticulateArguments(
		articulated.GetAbiEvent().Inputs,
		log.Data[2:],
		log.Topics,
		articulated.Inputs,
	)

	return
}
