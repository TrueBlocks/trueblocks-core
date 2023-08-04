package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	goEthAbi "github.com/ethereum/go-ethereum/accounts/abi"
)

func (abiCache *AbiCache) ArticulateTrace(chain string, trace *types.SimpleTrace) (err error) {
	address := trace.Action.To
	if !abiCache.loadedMap[address] && !abiCache.skipMap[address] {
		if err := abi.LoadAbi(chain, address, abiCache.abiMap); err != nil {
			abiCache.skipMap[address] = true
			return err
		} else {
			abiCache.loadedMap[address] = true
		}
	}

	if !abiCache.skipMap[address] {
		if trace.ArticulatedTrace, err = abiCache.articulateTrace(trace); err != nil {
			return err
		}
	}

	return nil
}

func (abiCache *AbiCache) articulateTrace(trace *types.SimpleTrace) (articulated *types.SimpleFunction, err error) {
	input := trace.Action.Input
	if len(input) < 10 {
		return
	}

	encoding := input[:10]
	articulated = abiCache.abiMap[encoding]

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
