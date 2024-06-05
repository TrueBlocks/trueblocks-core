package articulate

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/decode"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (abiCache *AbiCache) ArticulateTransaction(tx *types.Transaction) error {
	// TODO: This `Articulate` function is different from the other ones (for Log, etc.) because it
	// TODO: doesn't try to articulate the transaction by selector first.  It should.  But it doesn't.
	// TODO: The reason it doesn't is because of conflicting four-bytes (for example, `donate`)
	// if found, message, err := articulateTx(tx, &abiCache.AbiMap); err != nil {
	// 	return err
	//
	// } else if found != nil {
	// 	tx.ArticulatedTx = found
	// 	tx.Message = message
	//
	// } else {
	var err error
	address := tx.To
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
		if tx.ArticulatedTx, tx.Message, err = articulateTx(tx, &abiCache.AbiMap); err != nil {
			return err
		}
	} else {
		if message, ok := decode.ArticulateString(tx.Input); ok {
			tx.Message = message
		}
	}
	// }

	if err = abiCache.ArticulateReceipt(tx.Receipt); err != nil {
		return err
	}

	for index := range tx.Traces {
		if err = abiCache.ArticulateTrace(&tx.Traces[index]); err != nil {
			return err
		}
	}

	return nil
}

func articulateTx(tx *types.Transaction, abiMap *abi.SelectorSyncMap) (*types.Function, string, error) {
	var found *types.Function
	var message string
	var art *types.Function
	var selector string
	var input = tx.Input
	var outputData string
	if len(tx.Traces) > 0 && tx.Traces[0].Result != nil && len(tx.Traces[0].Result.Output) > 2 {
		outputData = tx.Traces[0].Result.Output[2:]
	}

	if len(input) >= 10 {
		selector = input[:10]
		inputData := input[10:]
		found = abiMap.GetValue(selector)
		if found != nil {
			art = found.Clone()
			if err := ArticulateFunction(art, inputData, outputData); err != nil {
				return found, "", err
			}
		}
	}

	if found == nil && len(input) > 0 {
		var ok bool
		var msg string
		if msg, ok = decode.ArticulateString(tx.Input); ok {
			message = msg
		}
	}

	return art, message, nil
}
