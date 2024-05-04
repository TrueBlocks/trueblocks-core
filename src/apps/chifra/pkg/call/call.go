package call

import (
	"errors"
	"fmt"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/parser"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var ErrAbiNotFound = errors.New("abi not found ")

type ContractCall struct {
	Conn        *rpc.Connection
	Address     base.Address
	Method      *types.Function
	Arguments   []any
	BlockNumber base.Blknum
	encoded     string
}

func NewContractCallWithAbi(conn *rpc.Connection, callAddress base.Address, theCall string, abiMap *abi.SelectorSyncMap) (*ContractCall, []string, error) {
	parsed, err := parser.ParseCall(theCall)
	if err != nil {
		err = fmt.Errorf("the value provided --call (%s) is invalid", theCall)
		return nil, []string{}, err
	}

	var function *types.Function
	var callArguments []*parser.ContractArgument
	suggestions := make([]string, 0)
	if parsed.Encoded != "" {
		selector := parsed.Encoded[:10]
		function, _, err = FindAbiFunction(FindBySelector, selector, nil, abiMap)
		if err != nil {
			return nil, []string{}, err
		}

	} else {
		// Selector or function name call
		findAbiMode := FindByName // default
		var identifier string

		switch {
		case parsed.FunctionNameCall != nil:
			findAbiMode = FindByName
			identifier = parsed.FunctionNameCall.Name
			callArguments = parsed.FunctionNameCall.Arguments
		case parsed.SelectorCall != nil:
			findAbiMode = FindBySelector
			identifier = parsed.SelectorCall.Selector.Value
			callArguments = parsed.SelectorCall.Arguments
		}

		function, suggestions, err = FindAbiFunction(findAbiMode, identifier, callArguments, abiMap)
		if err != nil {
			return nil, suggestions, err
		}
	}

	if function == nil {
		return nil, suggestions, fmt.Errorf("abi not found for %s: %s", theCall, ErrAbiNotFound)
	}

	var args []any
	if parsed.Encoded == "" {
		args, err = convertArguments(callArguments, function)
		if err != nil {
			return nil, suggestions, err
		}
	}

	contactCall := &ContractCall{
		Conn:      conn,
		Address:   callAddress,
		Method:    function,
		Arguments: args,
	}
	if parsed.Encoded != "" {
		contactCall.forceEncoding(parsed.Encoded)
	}

	return contactCall, suggestions, nil
}

func NewContractCall(conn *rpc.Connection, callAddress base.Address, theCall string) (*ContractCall, []string, error) {
	abiMap := &abi.SelectorSyncMap{}
	if err := abi.LoadAbi(conn, callAddress, abiMap); err != nil {
		return nil, []string{}, err
	}

	// TODO: Why does this not work?
	// return NewContractCallWithAbis(conn, callAddress, theCall, abiMap)

	parsed, err := parser.ParseCall(theCall)
	if err != nil {
		err = fmt.Errorf("the value provided --call (%s) is invalid", theCall)
		return nil, []string{}, err
	}

	var function *types.Function
	var callArguments []*parser.ContractArgument
	suggestions := make([]string, 0)
	if parsed.Encoded != "" {
		selector := parsed.Encoded[:10]
		function, _, err = FindAbiFunction(FindBySelector, selector, nil, abiMap)
		if err != nil {
			return nil, []string{}, err
		}

	} else {
		// Selector or function name call
		findAbiMode := FindByName
		var identifier string

		switch {
		case parsed.FunctionNameCall != nil:
			findAbiMode = FindByName
			identifier = parsed.FunctionNameCall.Name
			callArguments = parsed.FunctionNameCall.Arguments
		case parsed.SelectorCall != nil:
			findAbiMode = FindBySelector
			identifier = parsed.SelectorCall.Selector.Value
			callArguments = parsed.SelectorCall.Arguments
		}

		function, suggestions, err = FindAbiFunction(findAbiMode, identifier, callArguments, abiMap)
		if err != nil {
			return nil, suggestions, err
		}
	}

	if function == nil {
		return nil, suggestions, fmt.Errorf("abi not found for %s: %s", theCall, ErrAbiNotFound)
	}

	var args []any
	if parsed.Encoded == "" {
		args, err = convertArguments(callArguments, function)
		if err != nil {
			return nil, suggestions, err
		}
	}

	contactCall := &ContractCall{
		Conn:      conn,
		Address:   callAddress,
		Method:    function,
		Arguments: args,
	}
	if parsed.Encoded != "" {
		contactCall.forceEncoding(parsed.Encoded)
	}

	return contactCall, suggestions, nil
}

func convertArguments(callArguments []*parser.ContractArgument, function *types.Function) (args []any, err error) {
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

func (call *ContractCall) forceEncoding(encoding string) {
	call.encoded = encoding
}

func (call *ContractCall) Call(artFunc func(string, *types.Function) error) (results *types.Result, err error) {
	if artFunc == nil {
		logger.Fatal("should not happen ==> implementation error: artFunc is nil")
	}

	blockTs := base.Timestamp(0)
	if call.Conn.StoreReadable() {
		results = &types.Result{
			Address:     call.Address,
			BlockNumber: call.BlockNumber,
			Encoding:    call.Method.Encoding,
		}
		if err := call.Conn.Store.Read(results, nil); err == nil {
			// logger.Info("cache read:", results.Address, results.BlockNumber, call.Method.Encoding)
			return results, nil
			// } else {
			// 	logger.Info("no cache read:", results.Address, results.BlockNumber, call.Method.Encoding, err)
		}
		blockTs = call.Conn.GetBlockTimestamp(call.BlockNumber)
	}

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

	method := "eth_call"
	params := query.Params{
		map[string]any{
			"to":   call.Address.Hex(),
			"data": packedHex,
		},
		blockNumberHex,
	}

	rawBytes, err := query.Query[string](call.Conn.Chain, method, params)
	if err != nil {
		return nil, err
	}

	function := call.Method.Clone()
	// articulate it if possible
	if rawBytes != nil {
		str := *rawBytes
		if err = artFunc(str, function); err != nil {
			return nil, err
		}
	}

	results = &types.Result{
		BlockNumber:      call.BlockNumber,
		Timestamp:        blockTs,
		Address:          call.Address,
		Name:             call.Method.Name,
		Encoding:         call.Method.Encoding,
		Signature:        call.Method.Signature,
		EncodedArguments: encodedArguments,
		ReturnedBytes:    *rawBytes,
		ArticulatedOut:   function,
	}
	results.Values = make(map[string]string)
	for index, output := range function.Outputs {
		results.Values[output.DisplayName(index)] = fmt.Sprint(output.Value)
	}

	if call.Conn.StoreWritable() && call.Conn.EnabledMap["results"] && base.IsFinal(call.Conn.LatestBlockTimestamp, blockTs) {
		_ = call.Conn.Store.Write(results, nil)
		// logger.Info("Writing call results to the database...", results.Address, results.BlockNumber, call.Method.Encoding)
		// if err := call.Conn.Store.Write(results, nil); err != nil {
		// 	logger.Warn("Failed to write call results to the database", err) // report but don't fail
		// }
		// } else if !isFin {
		// 	logger.Info("Not caching result (not ripe)...", results.Address, results.BlockNumber, call.Method.Encoding)
	}

	return results, nil
}
