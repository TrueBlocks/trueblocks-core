package statePkg

import (
	"context"
	"errors"
	"fmt"
	"strings"

	abiPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/call"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/parser"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *StateOptions) HandleCall() error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	address := base.HexToAddress(opts.Addrs[0])
	callAddress := address
	if opts.ProxyFor != "" {
		callAddress = base.HexToAddress(opts.ProxyFor)
	}

	parsed, err := parser.ParseContractCall(opts.Call)
	if err != nil {
		// TODO: This is an end user error. It's meaningless to them. Only report what's required of the user.
		return fmt.Errorf("%w. The provided value (%s) must be a four-byte or function name followed by arguments, i.e. getBalance(), or full encoded data hash", err, opts.Call)
	}

	abiMap := make(abiPkg.AbiInterfaceMap)
	if err = abiPkg.LoadAbi(chain, address, abiMap); err != nil {
		return err
	}

	var function *types.SimpleFunction
	var callArguments []*parser.ContractCallArgument
	var suggestions []types.SimpleFunction

	if parsed.Encoded != "" {
		selector := parsed.Encoded[:10]
		function, _, err = findAbiFunction(findBySelector, selector, nil, abiMap)
		if err != nil {
			return err
		}
	} else {
		// Selector or function name call
		var findAbiMode findMode
		var identifier string

		switch {
		case parsed.FunctionNameCall != nil:
			findAbiMode = findByName
			identifier = parsed.FunctionNameCall.Name
			callArguments = parsed.FunctionNameCall.Arguments
		case parsed.SelectorCall != nil:
			findAbiMode = findBySelector
			identifier = parsed.SelectorCall.Selector.Value
			callArguments = parsed.SelectorCall.Arguments
		}

		function, suggestions, err = findAbiFunction(findAbiMode, identifier, callArguments, abiMap)
		if err != nil {
			return err
		}
	}

	if function == nil {
		message := fmt.Sprintf("No ABI found for function %s", opts.Call)
		if opts.Globals.IsApiMode() {
			return errors.New(message)
		}
		logger.Error(message)
		if len(suggestions) > 0 {
			logger.Info("Did you mean:")
			for index, suggestion := range suggestions {
				logger.Info(index+1, "-", suggestion.Signature)
			}
		}
		return nil
	}

	var args []any
	if parsed.Encoded == "" {
		args, err = convertArguments(callArguments, function)
		if err != nil {
			return err
		}
	}

	blocks := opts.Blocks
	if len(blocks) == 0 {
		blocks = []string{"latest"}
	}

	fetchData := func(modelChan chan types.Modeler[types.RawCallResult], errorChan chan error) {
		for _, blockExpression := range blocks {
			blockRange, err := identifiers.NewBlockRange(blockExpression)
			if err != nil {
				errorChan <- err
				return
			}

			resolvedBlock, err := blockRange.ResolveBlocks(chain)
			if err != nil {
				errorChan <- err
				return
			}

			for _, resolvedBlock := range resolvedBlock {
				contractCall := &call.ContractCall{
					Address:     callAddress,
					Method:      function,
					Arguments:   args,
					BlockNumber: resolvedBlock,
					ShowLogs:    opts.Globals.Verbose || testMode,
				}
				if parsed.Encoded != "" {
					contractCall.ForceEncoding(parsed.Encoded)
				}

				results, err := call.CallContract(
					chain,
					contractCall,
				)
				if err != nil {
					errorChan <- err
					return
				}

				modelChan <- results
			}
		}
	}

	return output.StreamMany(context.Background(), fetchData, opts.Globals.OutputOptsWithExtra(nil))
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

type findMode int

const (
	findByName findMode = iota
	findBySelector
)

// findAbiFunction returns either the function to call or a list of suggestions (functions
// with the same name, but different argument count)
func findAbiFunction(mode findMode, identifier string, arguments []*parser.ContractCallArgument, abis abiPkg.AbiInterfaceMap) (fn *types.SimpleFunction, suggestions []types.SimpleFunction, err error) {
	for _, function := range abis {
		function := function
		// TODO: is this too naive?
		if mode == findByName && function.Name != identifier {
			continue
		}
		if mode == findBySelector && function.Encoding != strings.ToLower(identifier) {
			continue
		}
		if arguments != nil && len(function.Inputs) != len(arguments) {
			suggestions = append(suggestions, *function)
			continue
		}

		return function, nil, nil
	}

	return nil, suggestions, nil
}
