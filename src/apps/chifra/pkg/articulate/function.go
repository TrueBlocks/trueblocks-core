package articulate

import (
	"encoding/hex"
	"encoding/json"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/accounts/abi"
	"github.com/ethereum/go-ethereum/common"
)

func ArticulateFunction(function *types.SimpleFunction, inputData string, outputData string) (err error) {
	abiMethod := function.GetAbiMethod()

	if len(inputData) > 0 {
		if err = ArticulateArguments(abiMethod.Inputs, inputData, nil, function.Inputs); err != nil {
			return fmt.Errorf("error processing inputs of %s: %w", abiMethod.Sig, err)
		}
	}

	if len(outputData) > 0 {
		if err = ArticulateArguments(abiMethod.Outputs, outputData, nil, function.Outputs); err != nil {
			return fmt.Errorf("error processing output of %s: %w", abiMethod.Sig, err)
		}
	}

	return
}

// func ArticulateArguments(args abi.Arguments, data string, topics []common.Hash, destination []types.SimpleParameter) (err error) {
// 	dataBytes, err := hex.DecodeString(data)
// 	if err != nil {
// 		return
// 	}
// 	unpacked, err := args.UnpackValues(dataBytes)
// 	nonIndexedArgs := args.NonIndexed()
// 	// unpacked := make(map[string]any, len(nonIndexedArgs))
// 	// err = args.UnpackIntoMap(unpacked, dataBytes)
// 	if err != nil {
// 		return
// 	}

// 	destinationArgNameToIndex := make(map[string]int, len(nonIndexedArgs))
// 	for index, destinationArg := range destination {
// 		destinationArgNameToIndex[destinationArg.Name] = index
// 	}
// 	for index, unpackedArg := range unpacked {
// 		currentArg := nonIndexedArgs[index]
// 		var result string
// 		switch currentArg.Type.T {
// 		case abi.TupleTy:
// 			fallthrough
// 		case abi.ArrayTy:
// 			fallthrough
// 		case abi.SliceTy:
// 			as := articulateSlice(&currentArg.Type, unpackedArg)
// 			if len(as) == 0 {
// 				result = fmt.Sprint(unpackedArg)
// 			} else {
// 				result = fmt.Sprint(as)
// 			}
// 		case abi.StringTy:
// 			fallthrough
// 		case abi.IntTy, abi.UintTy:
// 			fallthrough
// 		case abi.BoolTy:
// 			fallthrough
// 		case abi.BytesTy:
// 			result = fmt.Sprint(unpackedArg)
// 		case abi.FunctionTy:
// 			item, ok := unpackedArg.([]byte)
// 			if ok {
// 				result = common.Bytes2Hex(item)
// 				break
// 			}
// 			result = fmt.Sprint(unpackedArg)
// 		case abi.FixedBytesTy:
// 			result = articulateFixedBytes(&currentArg.Type, unpackedArg)
// 		case abi.AddressTy:
// 			fallthrough
// 		case abi.HashTy:
// 			result = strings.ToLower(fmt.Sprint(unpackedArg))
// 		default:
// 			return fmt.Errorf("cannot cast type %s", currentArg.Type.String())
// 		}
// 		destIndex, ok := destinationArgNameToIndex[currentArg.Name]
// 		if !ok {
// 			return fmt.Errorf("cannot find destination index of argument %s", currentArg.Name)
// 		}
// 		destination[destIndex].Value = result
// 	}
// 	return
// }

func ArticulateArguments(args abi.Arguments, data string, topics []common.Hash, destination []types.SimpleParameter) (err error) {
	dataBytes, err := hex.DecodeString(data)
	if err != nil {
		return
	}

	// First, we have to construct slices of indexed and non-indexed
	// arguments. We will use data for non-indexed ones and topics[1:]
	// for the indexed.
	// argNameToIndex will help us keep track on the argument index in
	// destination, as we will need to update the entry there
	argNameToIndex := make(map[string]int, len(args))
	nameToIndexed := make(map[string]abi.Argument)
	indexed := make(abi.Arguments, 0, len(args))
	nonIndexed := make(abi.Arguments, 0, len(args))
	for index, arg := range args {
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
		return
	}

	// Set values of non-indexed arguments
	for index, unpackedArg := range unpacked {
		currentArg := nonIndexed[index]
		result, err := valueToString(&currentArg.Type, unpackedArg)
		if err != nil {
			return err
		}
		destinationIndex, ok := argNameToIndex[currentArg.Name]
		if !ok {
			return fmt.Errorf("cannot find destination index of argument %s", currentArg.Name)
		}
		destination[destinationIndex].Value = result
	}

	if len(topics) == 0 {
		return
	}

	// Set values of indexed arguments
	out := make(map[string]interface{}, len(indexed))
	if err = abi.ParseTopicsIntoMap(out, indexed, topics[1:]); err != nil {
		return err
	}
	for name, value := range out {
		currentArg, ok := nameToIndexed[name]
		if !ok {
			return fmt.Errorf("cannot find indexed argument:%s", name)
		}
		result, err := valueToString(&currentArg.Type, value)
		if err != nil {
			return err
		}
		destinationIndex, ok := argNameToIndex[currentArg.Name]
		if !ok {
			return fmt.Errorf("cannot find destination index of argument %s", currentArg.Name)
		}
		destination[destinationIndex].Value = result
	}
	return
}

func valueToString(argType *abi.Type, value any) (result string, err error) {
	switch argType.T {
	case abi.TupleTy:
		// TODO(articulate): tuples can have elements of different types. Use argType.TupleTypes
		return articulateTuple(argType, value)
		// fallthrough
	case abi.ArrayTy:
		fallthrough
	case abi.SliceTy:
		articulatedSlice, err := articulateSlice(argType, value)
		if err != nil {
			return "", err
		}
		if len(articulatedSlice) == 0 {
			result = fmt.Sprint(value)
		} else {
			result = fmt.Sprint(articulatedSlice)
		}
	case abi.StringTy:
		fallthrough
	case abi.IntTy, abi.UintTy:
		fallthrough
	case abi.BoolTy:
		fallthrough
	case abi.BytesTy:
		result = fmt.Sprint(value)
	case abi.FunctionTy:
		item, ok := value.([]byte)
		if ok {
			result = common.Bytes2Hex(item)
			break
		}
		result = fmt.Sprint(value)
	case abi.FixedBytesTy:
		result = articulateFixedBytes(argType, value)
	case abi.AddressTy:
		fallthrough
	case abi.HashTy:
		result = strings.ToLower(fmt.Sprint(value))
	default:
		return "", fmt.Errorf("cannot cast type %s", argType.String())
	}

	return
}

func articulateTuple(argType *abi.Type, value any) (result string, err error) {
	asBytes, err := json.Marshal(value)
	if err == nil {
		result = string(asBytes)
	}
	return
}

func articulateSlice(sliceType *abi.Type, value interface{}) (result string, err error) {
	// TODO(articulation)
	switch sliceType.Elem.T {
	case abi.TupleTy:
		return articulateTuple(sliceType, value)
	// 	fallthrough
	// case abi.SliceTy:
	// 	// rec
	// 	fallthrough
	// case abi.ArrayTy:
	// 	// rec
	// 	fallthrough
	// case abi.StringTy:
	// 	fallthrough
	// case abi.IntTy, abi.UintTy:
	// 	fallthrough
	// case abi.BoolTy:
	// 	fallthrough
	// case abi.BytesTy:
	// 	fallthrough
	// case abi.FunctionTy:
	// 	fallthrough
	case abi.FixedBytesTy:
		if sliceType.Elem.Size == 20 {
			items, ok := value.([][20]byte)
			if !ok {
				return
			}
			strItems := make([]string, 0, len(items))
			for _, item := range items {
				addr := types.BytesToAddress(item[:])
				strItems = append(strItems, addr.Hex())
			}
			return fmt.Sprint(strItems), nil
		}

		if sliceType.Elem.Size == 32 {
			items, ok := value.([][32]byte)
			if !ok {
				return
			}
			strItems := make([]string, 0, len(items))
			for _, item := range items {
				hash := common.BytesToHash(item[:])
				strItems = append(strItems, strings.ToLower(hash.Hex()))
			}
			return fmt.Sprint(strItems), nil
		}
		return
	}
	return
}

func articulateFixedBytes(abiType *abi.Type, data any) string {
	addressLike, ok := data.([20]byte)
	if ok && abiType.Size == 20 {
		addr := types.BytesToAddress(addressLike[:])
		return addr.Hex()
	}

	hashLike, ok := data.([32]byte)
	if ok && abiType.Size == 32 {
		hash := common.BytesToHash(hashLike[:])
		return strings.ToLower(hash.Hex())
	}
	return fmt.Sprint(data)
}
