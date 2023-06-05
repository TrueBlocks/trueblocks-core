package statePkg

import (
	"errors"
	"fmt"

	abiPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/accounts/abi"
	"github.com/ethereum/go-ethereum/common"
)

func (opts *StateOptions) HandleCall() error {
	address := base.HexToAddress(opts.Addrs[0])

	parsed, err := Parse(opts.Call)
	if err != nil {
		return err
	}

	abiMap := make(abiPkg.AbiInterfaceMap)
	// TODO: maybe we don't need abi when the call is in "encoded" format. But return value?
	if err = abiPkg.LoadAbi(opts.Globals.Chain, address, abiMap); err != nil {
		return err
	}

	var packed []byte

	if parsed.Encoded != "" {
		packed = common.Hex2Bytes(parsed.Encoded)
	}

	var function *abi.Method

	if parsed.FunctionNameCall != nil {
		var suggestions []types.SimpleFunction
		function, suggestions, err = findAbiFunction(parsed.FunctionNameCall, abiMap)
		if err != nil {
			return err
		}
		if function == nil {
			logger.Error("No ABI found for function", opts.Call)
			if len(suggestions) > 0 {
				logger.Info("Did you mean:")
				for index, suggestion := range suggestions {
					logger.Info(index+1, "-", suggestion.Signature)
				}
			}
			return nil
		}

		packed, err = packFunction(parsed.FunctionNameCall, function)
		if err != nil {
			return err
		}
	}

	// TODO: check if we have packed

	raw, err := rpc.Query[string](opts.Globals.Chain, "eth_call", rpc.Params{
		map[string]any{
			"to":   address.Hex(),
			"data": "0x" + common.Bytes2Hex(packed),
		},
		// TODO: allow block number change
		"latest",
	})
	if err != nil {
		return err
	}
	value := *raw

	returned := make(map[string]interface{})
	err = function.Outputs.UnpackIntoMap(returned, common.Hex2Bytes(value[2:]))
	if err != nil {
		return fmt.Errorf("cannot unpack returned value: %s: %w", *raw, err)
	}
	for name, decodedValue := range returned {
		printName := name
		if name == "" {
			printName = "<anonymous>"
		}
		logger.Info(printName, decodedValue)
	}

	return nil
}

// findAbiFunction returns either the function to call or a list of suggestions (functions
// with the same name, but different argument count)
func findAbiFunction(call *FunctionCall, abis abiPkg.AbiInterfaceMap) (fn *abi.Method, suggestions []types.SimpleFunction, err error) {
	for _, function := range abis {
		function := function
		// TODO: is this too naive?
		if function.Name != call.Name {
			continue
		}
		if len(function.Inputs) != len(call.Arguments) {
			suggestions = append(suggestions, *function)
			continue
		}
		abiMethod, err := function.GetAbiMethod()
		if err != nil {
			return nil, nil, err
		}

		return abiMethod, nil, nil
	}

	return nil, suggestions, nil
}

// packFunction encodes function call
func packFunction(call *FunctionCall, function *abi.Method) (packed []byte, err error) {
	args := make([]interface{}, 0, len(call.Arguments))
	for index, arg := range call.Arguments {
		input := function.Inputs[index]

		if input.Type.T == abi.FixedBytesTy {
			// We only support fixed bytes as hex strings
			hex := *arg.Hex.String
			if len(hex) == 0 {
				return nil, errors.New("no value for fixed-size bytes argument")
			}

			arrayInterface, err := abi.ReadFixedBytes(input.Type, common.Hex2Bytes(hex[2:]))
			if err != nil {
				return nil, err
			}
			args = append(args, arrayInterface)
			continue
		}

		if input.Type.T == abi.IntTy {
			// We have to convert int64 to a correct int type, otherwise go-ethereum will
			// return an error. It's not needed for uints, because they handle them differently.
			if arg.Number.Big != nil {
				args = append(args, arg.Number.Interface())
				continue
			}

			converted, err := convertNumber(&input.Type, arg.Number)
			if err != nil {
				return nil, err
			}
			args = append(args, converted)
			continue
		}

		if input.Type.T == abi.AddressTy {
			// We need go-ethereum's Address type, not ours
			address := arg.Hex.Address
			if address == nil {
				return nil, errors.New("expected address")
			}

			a := common.HexToAddress(address.Hex())
			args = append(args, a)
			continue
		}

		args = append(args, arg.Interface())
	}

	packedArgs, err := function.Inputs.Pack(args...)
	if err != nil {
		return
	}
	packed = function.ID
	packed = append(packed, packedArgs...)

	return
}

func convertNumber(abiType *abi.Type, number *Number) (any, error) {
	if abiType.Size > 64 {
		return number.Big, nil
	}

	if abiType.T == abi.UintTy {
		switch abiType.Size {
		case 8:
			return uint8(*number.Uint), nil
		case 16:
			return uint16(*number.Uint), nil
		case 32:
			return uint32(*number.Uint), nil
		case 64:
			return uint64(*number.Uint), nil
		}
	} else if abiType.T == abi.IntTy {
		switch abiType.Size {
		case 8:
			return int8(*number.Int), nil
		case 16:
			return int16(*number.Int), nil
		case 32:
			return int32(*number.Int), nil
		case 64:
			return int64(*number.Int), nil
		}
	}

	return nil, fmt.Errorf("cannot convert %v to number", number.Interface())
}
