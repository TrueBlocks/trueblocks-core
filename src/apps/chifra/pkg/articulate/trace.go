package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	goEthAbi "github.com/ethereum/go-ethereum/accounts/abi"
)

func (cache *AbiCache) ArticulateTrace(chain string, trace *types.SimpleTrace) (err error) {
	address := trace.Action.To
	if !cache.loadedMap[address] && !cache.skipMap[address] {
		if err := abi.LoadAbi(chain, address, cache.abiMap); err != nil {
			cache.skipMap[address] = true
			return err
		} else {
			cache.loadedMap[address] = true
		}
	}

	if !cache.skipMap[address] {
		if trace.ArticulatedTrace, err = articulateTrace(trace, cache.abiMap); err != nil {
			return err
		}
	}

	return nil
}

func articulateTrace(trace *types.SimpleTrace, abiMap abi.AbiInterfaceMap) (articulated *types.SimpleFunction, err error) {
	input := trace.Action.Input
	if len(input) < 10 {
		return
	}
	encoding := input[:10]
	articulated = abiMap[encoding]

	if trace.Result == nil || articulated == nil {
		return
	}

	var abiMethod *goEthAbi.Method

	if len(trace.Action.Input) > 10 {
		abiMethod, err = articulated.GetAbiMethod()
		if err != nil {
			return nil, err
		}
		err = ArticulateArguments(
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
	err = ArticulateArguments(
		abiMethod.Outputs,
		trace.Result.Output[2:],
		nil,
		articulated.Outputs,
	)

	return
}
