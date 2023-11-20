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
	if found, err := articulateLogFromMap(log, &abiCache.AbiMap); err != nil {
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
			if log.ArticulatedLog, err = articulateLogFromMap(log, &abiCache.AbiMap); err != nil {
				return err
			}
		}
		return nil
	}
}

func articulateLogFromMap(log *types.SimpleLog, abiMap *abi.SelectorSyncMap) (*types.SimpleFunction, error) {
	if len(log.Topics) < 1 {
		return nil, nil
	}

	// Try to articulate the log using some common events
	artLog := findCommonEvent(log)

	// If we couldn't, then try to find the event in `abiMap`
	if artLog == nil {
		selector := "0x" + hex.EncodeToString(log.Topics[0].Bytes())
		if found := abiMap.GetValue(selector); found != nil {
			artLog = found.Clone()
		} else {
			// If artLog is still nil, we don't have ABI for this event
			return nil, nil
		}
	}

	abiEvent, err := artLog.GetAbiEvent()
	if err != nil {
		return artLog, err
	}
	data := log.Data
	if len(log.Data) > 1 {
		data = log.Data[2:]
	}

	if err = articulateArguments(abiEvent.Inputs, data, log.Topics, artLog.Inputs); err != nil {
		return artLog, err
	}

	return artLog, nil
}

func findCommonEvent(log *types.SimpleLog) *types.SimpleFunction {
	if artLog := parseTransferEvent(log); artLog != nil {
		return artLog

	} else if artLog = parseEnsTransferEvent(log); artLog != nil {
		return artLog

	} else if artLog = parseApprovalEvent(log); artLog != nil {
		return artLog
	}

	return nil
}
