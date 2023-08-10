package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	goEthAbi "github.com/ethereum/go-ethereum/accounts/abi"
)

func (abiCache *AbiCache) ArticulateTrace(trace *types.SimpleTrace) (err error) {
	address := trace.Action.To
	if !abiCache.loadedMap.GetValue(address) && !abiCache.skipMap.GetValue(address) {
		if err := abi.LoadAbi(abiCache.Chain, address, &abiCache.AbiMap); err != nil {
			abiCache.skipMap.SetValue(address, true)
			return err
		} else {
			abiCache.loadedMap.SetValue(address, true)
		}
	}

	if !abiCache.skipMap.GetValue(address) {
		if trace.ArticulatedTrace, err = articulateTrace(trace, &abiCache.AbiMap); err != nil {
			return err
		}
	}

	return nil
}

func articulateTrace(trace *types.SimpleTrace, abiMap *abi.FunctionSyncMap) (articulated *types.SimpleFunction, err error) {
	input := trace.Action.Input
	if len(input) < 10 {
		return
	}

	encoding := input[:10]
	articulated = abiMap.GetValue(encoding)

	if trace.Result == nil || articulated == nil {
		return
	}

	var abiMethod *goEthAbi.Method

	if len(trace.Action.Input) > 10 {
		abiMethod, err = articulated.GetAbiMethod()
		if err != nil {
			return nil, err
		}
		err = articulateArguments(
			abiMethod.Inputs,
			trace.Action.Input[10:],
			nil,
			articulated.Inputs,
		)
		if err != nil {
			return
		}
	}

	abiMethod, err = articulated.GetAbiMethod()
	if err != nil {
		return nil, err
	}
	err = articulateArguments(
		abiMethod.Outputs,
		trace.Result.Output[2:],
		nil,
		articulated.Outputs,
	)

	return
}
