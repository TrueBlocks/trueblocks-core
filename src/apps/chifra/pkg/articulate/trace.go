package articulate

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	goEthAbi "github.com/ethereum/go-ethereum/accounts/abi"
)

func (abiCache *AbiCache) ArticulateTrace(trace *types.Trace) (err error) {
	found, err := articulateTrace(trace, &abiCache.AbiMap)
	if err != nil {
		return err

	} else if found != nil {
		trace.ArticulatedTrace = found
		return nil

	} else {
		address := trace.Action.To
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
			if trace.ArticulatedTrace, err = articulateTrace(trace, &abiCache.AbiMap); err != nil {
				return err
			}
		}

		return nil
	}
}

func articulateTrace(trace *types.Trace, abiMap *abi.SelectorSyncMap) (articulated *types.Function, err error) {
	input := trace.Action.Input
	if len(input) < 10 {
		return
	}

	encoding := input[:10]
	found := abiMap.GetValue(encoding)

	if trace.Result == nil || found == nil {
		return
	}

	// Clone the function to avoid modifying the cached instance
	articulated = found.Clone()

	var abiMethod *goEthAbi.Method

	if len(trace.Action.Input) >= 10 {
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
	if len(trace.Result.Output) >= 2 {
		err = articulateArguments(
			abiMethod.Outputs,
			trace.Result.Output[2:],
			nil,
			articulated.Outputs,
		)
	}

	return
}
