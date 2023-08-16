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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type ContractCall struct {
	Conn        *rpc.Connection
	Address     base.Address
	Method      *types.SimpleFunction
	Arguments   []any
	BlockNumber uint64
	encoded     string
}

func NewContractCall(conn *rpc.Connection, callAddress base.Address, theCall string, showSuggestions bool) (*ContractCall, error) {
	parsed, err := parser.ParseContractCall(theCall)
	if err != nil {
		err = fmt.Errorf("the value provided --call (%s) is invalid", theCall)
		return nil, err
	}

	abiMap := abi.NewFunctionSyncMap()
	if err = abi.LoadAbi(conn.Chain, callAddress, abiMap); err != nil {
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
		Conn:      conn,
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

func (call *ContractCall) forceEncoding(encoding string) {
	call.encoded = encoding
}

func (call *ContractCall) Call12() (results *types.SimpleCallResult, err error) {
	blockTs := base.Timestamp(0)
	if call.Conn.StoreReadable() {
		results = &types.SimpleCallResult{
			Address:     call.Address,
			BlockNumber: call.BlockNumber,
		}
		if err := call.Conn.Store.Read(results, nil); err == nil {
			// logger.Info("Read from the database...", results.Address, results.BlockNumber)
			return results, nil
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

	rawReturn, err := query.Query[string](call.Conn.Chain, "eth_call", query.Params{
		map[string]any{
			"to":   call.Address.Hex(),
			"data": packedHex,
		},
		blockNumberHex,
	})
	if err != nil {
		return nil, err
	}

	function := call.Method.Clone()

	rr := *rawReturn
	if len(rr) > 2 {
		abiCache := articulate.NewAbiCache(call.Conn.Chain, true)
		err = abiCache.ArticulateFunction(function, "", rr[2:])
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

	if call.Conn.StoreWritable() && call.Conn.EnabledMap["callresults"] && isFinal(call.Conn.LatestBlockTimestamp, blockTs) {
		// logger.Info("Writing call results to the database...", results.Address, results.BlockNumber)
		_ = call.Conn.Store.Write(results, nil) // ; err != nil {
		// logger.Warn("Failed to write call results to the database", err) // report but don't fail
		// }
	}

	return results, nil
}

func isFinal(latestTs, blockTs base.Timestamp) bool {
	// TODO: This is not consistent with they way we determine unripe in the scraper, for example.
	var pendingPeriod = int64(5 * 60)
	return (latestTs - blockTs) >= pendingPeriod
}
