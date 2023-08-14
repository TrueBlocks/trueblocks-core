package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/decode"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (abiCache *AbiCache) ArticulateTransaction(tx *types.SimpleTransaction) (err error) {
	address := tx.To
	if !abiCache.loadedMap.GetValue(address) && !abiCache.skipMap.GetValue(address) {
		if err := abi.LoadAbi(abiCache.Chain, address, &abiCache.AbiMap); err != nil {
			abiCache.skipMap.SetValue(address, true)
			return err
		} else {
			abiCache.loadedMap.SetValue(address, true)
		}
	}

	if tx.Receipt != nil {
		for index := range tx.Receipt.Logs {
			if err = abiCache.ArticulateLog(&tx.Receipt.Logs[index]); err != nil {
				return err
			}
		}
	}

	for index := range tx.Traces {
		if err = abiCache.ArticulateTrace(&tx.Traces[index]); err != nil {
			return err
		}
	}

	var found *types.SimpleFunction
	var selector string
	if len(tx.Input) >= 10 {
		selector = tx.Input[:10]
		inputData := tx.Input[10:]
		found = abiCache.AbiMap.GetValue(selector)
		if found != nil {
			tx.ArticulatedTx = found.Clone()
			var outputData string
			if len(tx.Traces) > 0 && tx.Traces[0].Result != nil && len(tx.Traces[0].Result.Output) > 2 {
				outputData = tx.Traces[0].Result.Output[2:]
			}
			if err = abiCache.ArticulateFunction(tx.ArticulatedTx, inputData, outputData); err != nil {
				return err
			}
		}
	}

	if found == nil && len(tx.Input) > 0 {
		if message, ok := decode.ArticulateString(tx.Input); ok {
			tx.Message = message
		}
	}

	return nil
}
