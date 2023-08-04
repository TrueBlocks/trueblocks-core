package call

import (
	"errors"
	"fmt"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/parser"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type ContractCall struct {
	encoded string

	Address     base.Address
	Method      *types.SimpleFunction
	Arguments   []any
	BlockNumber uint64
	ShowLogs    bool
}

func NewContractCall(chain string, callAddress base.Address, theCall string, showSuggestions bool) (*ContractCall, error) {
	parsed, err := parser.ParseContractCall(theCall)
	if err != nil {
		err = fmt.Errorf("the value provided --call (%s) is invalid", theCall)
		return nil, err
	}

	abiMap := abi.NewFunctionSyncMap()
	if err = abi.LoadAbi(chain, callAddress, abiMap); err != nil {
		return nil, err
	}

	var function *types.SimpleFunction
	var callArguments []*parser.ContractCallArgument
	var suggestions []types.SimpleFunction

	if parsed.Encoded != "" {
		selector := parsed.Encoded[:10]
		function, _, err = abi.FindAbiFunction(abi.FindBySelector, selector, nil, abiMap)
		if err != nil {
			return nil, err
		}

	} else {
		// Selector or function name call
		var findAbiMode abi.FindMode
		var identifier string

		switch {
		case parsed.FunctionNameCall != nil:
			findAbiMode = abi.FindByName
			identifier = parsed.FunctionNameCall.Name
			callArguments = parsed.FunctionNameCall.Arguments
		case parsed.SelectorCall != nil:
			findAbiMode = abi.FindBySelector
			identifier = parsed.SelectorCall.Selector.Value
			callArguments = parsed.SelectorCall.Arguments
		}

		function, suggestions, err = abi.FindAbiFunction(findAbiMode, identifier, callArguments, abiMap)
		if err != nil {
			return nil, err
		}
	}

	if function == nil {
		message := fmt.Sprintf("No ABI found for function %s", theCall)
		if showSuggestions {
			logger.Error(message)
			if len(suggestions) > 0 {
				logger.Info("Did you mean:")
				for index, suggestion := range suggestions {
					logger.Info(index+1, "-", suggestion.Signature)
				}
			}
		}
		return nil, errors.New(message)
	}

	var args []any
	if parsed.Encoded == "" {
		args, err = convertArguments(callArguments, function)
		if err != nil {
			return nil, err
		}
	}

	contactCall := &ContractCall{
		Address:   callAddress,
		Method:    function,
		Arguments: args,
	}
	if parsed.Encoded != "" {
		contactCall.forceEncoding(parsed.Encoded)
	}

	return contactCall, nil
}

func convertArguments(callArguments []*parser.ContractCallArgument, function *types.SimpleFunction) (args []any, err error) {
	abiMethod, err := function.GetAbiMethod()
	if err != nil {
		return
	}
	if len(abiMethod.Inputs) != len(callArguments) {
		return nil, fmt.Errorf("got %d argument(s), but wanted %d", len(abiMethod.Inputs), len(callArguments))
	}

	args = make([]any, 0, len(callArguments))
	for index, arg := range callArguments {
		converted, err := arg.AbiType(&abiMethod.Inputs[index].Type)
		if err != nil {
			return nil, err
		}
		args = append(args, converted)
	}

	return
}

func (c *ContractCall) forceEncoding(encoding string) {
	c.encoded = encoding
}

func CallContract(chain string, call *ContractCall) (results *types.SimpleCallResult, err error) {
	blockNumberHex := "0x" + strconv.FormatUint(call.BlockNumber, 16)
	if err != nil {
		return
	}

	var packed []byte
	if call.encoded != "" {
		packed = base.Hex2Bytes(call.encoded[2:])
	} else {
		packed, err = call.Method.Pack(call.Arguments)
		if err != nil {
			return nil, err
		}
	}

	packedHex := "0x" + base.Bytes2Hex(packed)
	encodedArguments := ""
	if len(packedHex) > 10 {
		encodedArguments = packedHex[10:]
	}

	rawReturn, err := rpc.Query[string](chain, "eth_call", rpc.Params{
		map[string]any{
			"to":   call.Address.Hex(),
			"data": packedHex,
		},
		blockNumberHex,
	})
	if err != nil {
		return nil, err
	}

	rr := *rawReturn
	function := call.Method.Clone()
	if len(rr) > 2 {
		err = articulate.ArticulateFunction(function, "", rr[2:])
		if err != nil {
			return nil, err
		}
	}

	results = &types.SimpleCallResult{
		BlockNumber:      call.BlockNumber,
		Address:          call.Address,
		Name:             call.Method.Name,
		Encoding:         call.Method.Encoding,
		Signature:        call.Method.Signature,
		EncodedArguments: encodedArguments,
		RawReturn:        *rawReturn,
	}
	results.Outputs = make(map[string]string)

	for index, output := range function.Outputs {
		results.Outputs[output.DisplayName(index)] = fmt.Sprint(output.Value)
	}

	if call.ShowLogs {
		msg := fmt.Sprintf("call to %s at block %d at four-byte %s returned %v",
			call.Address.Hex(), call.BlockNumber, call.Method.Encoding, results.Outputs)
		logger.TestLog(true, msg)
	}

	return results, nil
}
