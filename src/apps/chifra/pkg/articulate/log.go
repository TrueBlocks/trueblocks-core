package articulate

import (
	"encoding/hex"
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/normalize"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/topics"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// ArticulateLog articulates a log by attaching the Articulated log structure if the ABI is found.
func (abiCache *AbiCache) ArticulateLog(log *types.Log) error {
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

func articulateLogFromMap(log *types.Log, abiMap *abi.SelectorSyncMap) (*types.Function, error) {
	if len(log.Topics) < 1 {
		return nil, nil
	}

	// Try to articulate the log using some common events
	artLog, err := findCommonEvent(log)
	if err != nil {
		return nil, err
	}

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

func findCommonEvent(log *types.Log) (*types.Function, error) {
	if !topics.KnownTopics[log.Topics[0]] {
		return nil, nil
	}

	if normalized, _, err := normalize.NormalizeKnownLogs(log); err != nil {
		return nil, err

	} else {
		switch normalized.Topics[0] {
		case topics.TransferTopic:
			return parseTransferEvent(normalized), nil
		case topics.ApprovalTopic:
			return parseApprovalEvent(normalized), nil
		case topics.EnsTransferTopic:
			return parseEnsTransferEvent(normalized), nil
		}
		return nil, fmt.Errorf("parseFunc(commonEvent): target is not of correct type %v", log.Topics[0])
	}
}
