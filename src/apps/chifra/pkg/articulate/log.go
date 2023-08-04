package articulate

import (
	"encoding/hex"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (abiCache *AbiCache) ArticulateLog(chain string, log *types.SimpleLog) (err error) {
	address := log.Address
	if !abiCache.loadedMap.Get(address) && !abiCache.skipMap.Get(address) {
		if err = abi.LoadAbi(chain, address, &abiCache.abiMap); err != nil {
			abiCache.skipMap.Set(address, true)
			return err
		} else {
			abiCache.loadedMap.Set(address, true)
		}
	}

	if !abiCache.skipMap.Get(address) {
		if log.ArticulatedLog, err = articulateLog(log, &abiCache.abiMap); err != nil {
			return err
		}
	}

	return nil
}

func articulateLog(log *types.SimpleLog, abiMap *abi.FunctionSyncMap) (articulated *types.SimpleFunction, err error) {
	if len(log.Topics) < 1 {
		return
	}

	// Try to articulate the log using some common events
	articulated = findCommonEvent(log)

	// If we couldn't, then try to find the event in `abiMap`
	if articulated == nil {
		selector := "0x" + hex.EncodeToString(log.Topics[0].Bytes())
		if found := abiMap.Get(selector); found != nil {
			articulated = found.Clone()
		} else {
			// If articulated is still nil, we don't have ABI for this event
			return
		}
	}

	abiEvent, err := articulated.GetAbiEvent()
	if err != nil {
		return
	}
	data := log.Data
	if len(log.Data) > 1 {
		data = log.Data[2:]
	}

	if err = ArticulateArguments(
		abiEvent.Inputs,
		data,
		log.Topics,
		articulated.Inputs,
	); err != nil {
		return
	}

	return
}

func findCommonEvent(log *types.SimpleLog) (articulated *types.SimpleFunction) {
	if articulated = ParseTransferEvent(log); articulated != nil {
		return
	}
	if articulated = ParseEnsTransferEvent(log); articulated != nil {
		return
	}
	if articulated = ParseApprovalEvent(log); articulated != nil {
		return
	}
	return
}
