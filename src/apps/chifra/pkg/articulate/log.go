package articulate

import (
	"encoding/hex"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// ArticulateLog articulates a log by attaching the Articulated log structure if the ABI is found.
func (abiCache *AbiCache) ArticulateLog(log *types.SimpleLog) error {
	if found, err := articulateLog(log, &abiCache.AbiMap); err != nil {
		return err

	} else if found != nil {
		log.ArticulatedLog = found
		return nil

	} else {
		address := log.Address
		if !abiCache.loadedMap.GetValue(address) && !abiCache.skipMap.GetValue(address) {
			if err = abi.LoadAbi(abiCache.Conn, address, &abiCache.AbiMap); err != nil {
				abiCache.skipMap.SetValue(address, true)
				if !errors.Is(err, rpc.ErrNotAContract) {
					// Not being a contract is not an error because we want to articulate the input in case it's a message
					return err
				}
			} else {
				abiCache.loadedMap.SetValue(address, true)
			}
		}

		if !abiCache.skipMap.GetValue(address) {
			if log.ArticulatedLog, err = articulateLog(log, &abiCache.AbiMap); err != nil {
				return err
			}
		}
		return nil
	}
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
		if found := abiMap.GetValue(selector); found != nil {
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

	if err = articulateArguments(
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
	if articulated = parseTransferEvent(log); articulated != nil {
		return
	}
	if articulated = parseEnsTransferEvent(log); articulated != nil {
		return
	}
	if articulated = parseApprovalEvent(log); articulated != nil {
		return
	}
	return
}
