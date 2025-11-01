package call

import (
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/parser"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/walk"
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
		err = fmt.Errorf("the value provided (%s) is invalid", theCall)
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
		err = fmt.Errorf("the value provided (%s) is invalid", theCall)
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
	results = &types.Result{
		BlockNumber: call.BlockNumber,
		Address:     call.Address,
		Encoding:    call.Method.Encoding,
	}
	if err := call.Conn.ReadFromCache(results); err == nil {
		return results, nil
	}

	blockTs := call.Conn.GetBlockTimestamp(call.BlockNumber)
	if artFunc == nil {
		logger.ShouldNotHappen("artFunc is nil")
	}

	var packed []byte
	blockNumberHex := fmt.Sprintf("0x%x", call.BlockNumber)
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

	theBytes, err := query.Query[string](call.Conn.Chain, method, params)
	if err != nil {
		return nil, err
	}

	function := call.Method.Clone()
	// articulate it if possible
	if theBytes != nil {
		str := *theBytes
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
		ReturnedBytes:    *theBytes,
		ArticulatedOut:   function,
	}
	results.Values = make(map[string]string)
	for index, output := range function.Outputs {
		results.Values[output.DisplayName(index)] = fmt.Sprint(output.Value)
	}

	err = call.Conn.WriteToCache(results, walk.Cache_Results, blockTs)
	return results, err
}
