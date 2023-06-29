package articulate

import (
	"encoding/hex"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (cache *AbiCache) ArticulateLog(chain string, log *types.SimpleLog) (err error) {
	address := log.Address
	if !cache.loadedMap[address] && !cache.skipMap[address] {
		if err = abi.LoadAbi(chain, address, cache.abiMap); err != nil {
			cache.skipMap[address] = true
			return err
		} else {
			cache.loadedMap[address] = true
		}
	}

	if !cache.skipMap[address] {
		if log.ArticulatedLog, err = articulateLog(log, cache.abiMap); err != nil {
			return err
		}
	}

	return nil
}

func articulateLog(log *types.SimpleLog, abiMap abi.AbiInterfaceMap) (articulated *types.SimpleFunction, err error) {
	// Try to articulate the log using some common events
	articulated = findCommonEvent(log)

	// If we couldn't, then try to find the event in `abiMap`
	if articulated == nil {
		selector := "0x" + hex.EncodeToString(log.Topics[0].Bytes())

		if found := abiMap[selector]; found != nil {
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
	data := log.Data[2:]
	err = ArticulateArguments(
		abiEvent.Inputs,
		data,
		log.Topics,
		articulated.Inputs,
	)

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
