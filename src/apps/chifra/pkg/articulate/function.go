package articulate

import (
	"encoding/hex"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/accounts/abi"
)

func ArticulateFunction(function *types.SimpleFunction, inputData string, outputData string) (err error) {
	abiMethod := function.GetAbiMethod()

	if len(inputData) > 0 {
		if err = ArticulateArguments(abiMethod.Inputs, inputData, function.Inputs); err != nil {
			return err
		}
	}
	if len(outputData) > 0 {
		if err = ArticulateArguments(abiMethod.Outputs, outputData, function.Outputs); err != nil {
			return err
		}
	}

	return
}

func ArticulateArguments(args abi.Arguments, data string, destination []types.SimpleParameter) (err error) {
	dataBytes, err := hex.DecodeString(data)
	if err != nil {
		return
	}
	unpacked, err := args.Unpack(dataBytes)
	if err != nil {
		return
	}

	for index, unpackedArg := range unpacked {
		var result string
		switch args[index].Type.T {
		case abi.TupleTy:
			fallthrough
		case abi.SliceTy:
			fallthrough
		case abi.ArrayTy:
			fallthrough
		case abi.StringTy:
			fallthrough
		case abi.IntTy, abi.UintTy:
			fallthrough
		case abi.BoolTy:
			fallthrough
		case abi.BytesTy:
			fallthrough
		case abi.FunctionTy:
			fallthrough
		case abi.FixedBytesTy:
			result = fmt.Sprint(unpackedArg)
		case abi.AddressTy:
			fallthrough
		case abi.HashTy:
			result = strings.ToLower(fmt.Sprint(unpackedArg))
		default:
			return fmt.Errorf("cannot cast type %s", args[index].Type.String())
		}
		destination[index].Value = result
	}
	return
}
