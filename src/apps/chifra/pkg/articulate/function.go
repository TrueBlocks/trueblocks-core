package articulate

import (
	"encoding/hex"
	"fmt"
	"reflect"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/decode"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/accounts/abi"
	"github.com/ethereum/go-ethereum/common"
)

func ArticulateFunction(function *types.SimpleFunction, inputData string, outputData string) (err error) {
	abiMethod, err := function.GetAbiMethod()
	if err != nil {
		return
	}

	if len(inputData) > 0 {
		if err = articulateArguments(abiMethod.Inputs, inputData, nil, function.Inputs); err != nil {
			return fmt.Errorf("error processing inputs of %s: %w", abiMethod.Sig, err)
		}
	}

	if len(outputData) > 0 {
		if err = articulateArguments(abiMethod.Outputs, outputData, nil, function.Outputs); err != nil {
			return fmt.Errorf("error processing output of %s: %w", abiMethod.Sig, err)
		}
	}

	return
}

func articulateArguments(args abi.Arguments, data string, topics []base.Hash, abiMap []types.SimpleParameter) (err error) {
	dataBytes, err := hex.DecodeString(data)
	if err != nil {
		return
	}

	// First, we have to construct slices of indexed and non-indexed
	// arguments. We will use data for non-indexed ones and topics[1:]
	// for the indexed.
	// argNameToIndex will help us keep track on the argument index in
	// abiMap, as we will need to update the entry there
	argNameToIndex := make(map[string]int, len(args))
	nameToIndexed := make(map[string]abi.Argument)
	indexed := make(abi.Arguments, 0, len(args))
	nonIndexed := make(abi.Arguments, 0, len(args))

	// In some cases, `data` can be too short, because only some values are present there.
	// See: https://github.com/TrueBlocks/trueblocks-core/issues/1366
	dataLen := len(data) * 4
	nonIndexedSize := 0
	for _, nonIndexedArg := range args.NonIndexed() {
		nonIndexedSize += nonIndexedArg.Type.Size
	}

	for i := 0; i < len(args); i++ {
		index := i
		if dataLen < nonIndexedSize {
			fixFalseNonIndexedArgument(&args[i], dataLen)
		}
		arg := args[i]
		argNameToIndex[arg.Name] = index
		if arg.Indexed {
			indexed = append(indexed, arg)
			nameToIndexed[arg.Name] = arg
			continue
		}
		nonIndexed = append(nonIndexed, arg)
	}

	unpacked, err := args.Unpack(dataBytes)
	if err != nil {
		return fmt.Errorf("error unpacking arguments %w", err)
	}

	// Set values of non-indexed arguments
	for index, unpackedArg := range unpacked {
		currentArg := nonIndexed[index]
		result, err := formatValue(&currentArg.Type, unpackedArg)
		if err != nil {
			return err
		}
		var abiMapIndex int
		if currentArg.Name != "" {
			var ok bool
			abiMapIndex, ok = argNameToIndex[currentArg.Name]
			if !ok {
				return fmt.Errorf("cannot find abiMap index of argument %s", currentArg.Name)
			}
		} else {
			abiMapIndex = index
		}
		abiMap[abiMapIndex].Value = result
	}

	// Sometimes there are topics, but no indexed parameters:
	// https://github.com/TrueBlocks/trueblocks-core/issues/1366
	if len(topics) == 0 || len(indexed) == 0 {
		return
	}

	// Set values of indexed arguments
	out := make(map[string]interface{}, len(indexed))
	tops := []common.Hash{}
	for _, hash := range topics {
		tops = append(tops, hash.Common())
	}
	if err = abi.ParseTopicsIntoMap(out, indexed, tops[1:]); err != nil {
		return err
	}
	for name, value := range out {
		currentArg, ok := nameToIndexed[name]
		if !ok {
			return fmt.Errorf("cannot find indexed argument:%s", name)
		}
		result, err := formatValue(&currentArg.Type, value)
		if err != nil {
			return err
		}
		abiMapIndex, ok := argNameToIndex[currentArg.Name]
		if !ok {
			return fmt.Errorf("cannot find abiMap index of argument %s", currentArg.Name)
		}
		abiMap[abiMapIndex].Value = result
	}
	return
}

// fixFalseNonIndexedArgument turns false non-indexed argument to indexed one
func fixFalseNonIndexedArgument(arg *abi.Argument, dataLen int) {
	if arg.Indexed {
		return
	}

	if dataLen == arg.Type.Size {
		return
	}
	arg.Indexed = true
}

func formatValue(argType *abi.Type, value any) (result any, err error) {
	switch argType.T {
	case abi.TupleTy:
		tuple := reflect.ValueOf(value)
		numField := tuple.NumField()
		articulatedTuple := make(map[string]any, numField)
		for i := 0; i < numField; i++ {
			fieldValue := tuple.Field(i).Interface()
			itemAbiType := argType.TupleElems[i]
			articulatedItem, err := formatValue(itemAbiType, fieldValue)
			if err != nil {
				return "", nil
			}
			articulatedTuple[argType.TupleRawNames[i]] = articulatedItem
		}
		return articulatedTuple, nil
	case abi.ArrayTy:
		fallthrough
	case abi.SliceTy:
		slice := reflect.ValueOf(value)
		articulatedSlice := make([]any, 0, slice.Len())
		for i := 0; i < slice.Len(); i++ {
			item := slice.Index(i).Interface()
			articulatedItem, err := formatValue(argType.Elem, item)
			if err != nil {
				return "", err
			}
			articulatedSlice = append(articulatedSlice, articulatedItem)
		}
		result = articulatedSlice
	case abi.StringTy:
		strValue, ok := value.(string)
		if ok {
			return decode.SanitizeString(strValue), nil
		}
		fallthrough
	case abi.IntTy, abi.UintTy:
		// Because values coming from Ethereum can exceed JSON maximum safe integer, we
		// return all numbers as strings.
		// We could return numbers or strings depending on whether or not the value is safe,
		// but the consumer would then have to check the type of returned value first and only
		// then try to interpet the actual value.
		// In at least some languages, strings can be used to create big Int equivalents, so
		// using strings only should make it easier for the consumers.
		result = fmt.Sprint(value)
	case abi.BoolTy:
		result = value
	case abi.BytesTy:
		result = fmt.Sprintf("0x%x", value)
	case abi.FunctionTy:
		item, ok := value.([]byte)
		if ok {
			result = base.Bytes2Hex(item)
			break
		}
		result = value
	case abi.FixedBytesTy:
		result = articulateFixedBytes(argType, value)
	case abi.AddressTy:
		addr, ok := value.(common.Address)
		if !ok {
			result = fmt.Sprint(value)
			return
		}
		ourAddr := base.HexToAddress(addr.Hex())
		result = ourAddr.Hex()
	case abi.HashTy:
		result = strings.ToLower(fmt.Sprint(value))
	default:
		return "", fmt.Errorf("cannot cast type %s", argType.String())
	}

	return
}

func articulateFixedBytes(abiType *abi.Type, data any) string {
	addressLike, ok := data.([20]byte)
	if ok && abiType.Size == 20 {
		addr := base.BytesToAddress(addressLike[:])
		return addr.Hex()
	}

	hashLike, ok := data.([32]byte)
	if ok && abiType.Size == 32 {
		hash := base.BytesToHash(hashLike[:])
		articulated, ok := decode.ArticulateString(hash.Hex())
		if ok {
			return articulated
		}
		return hash.Hex()
	}

	return fmt.Sprint(data)
}
