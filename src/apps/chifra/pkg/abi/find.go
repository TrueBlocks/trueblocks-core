package abi

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/parser"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type FindMode int

const (
	FindByName FindMode = iota
	FindBySelector
)

// FindAbiFunction returns either the function to call or a list of suggestions (function
// signatures with the same name, but different argument count)
func FindAbiFunction(mode FindMode, identifier string, arguments []*parser.ContractCallArgument, abiMap *FunctionSyncMap) (fn *types.SimpleFunction, suggestions []string, err error) {
	functions := abiMap.Values()
	for _, function := range functions {
		function := function

		if (mode == FindByName && function.Name != identifier) ||
			(mode == FindBySelector && function.Encoding != strings.ToLower(identifier)) {
			continue
		}

		// now we will compare arguments
		if arguments != nil {
			// we start with argument count
			if len(function.Inputs) != len(arguments) {
				suggestions = append(suggestions, function.Signature)
				continue
			}
			// now we check if the argument types are compatible
			compatible := false
			for index, input := range function.Inputs {
				argument := arguments[index]
				if input.InternalType == "string" && argument.String == nil {
					break
				}
				if input.InternalType == "bool" && argument.Boolean == nil {
					break
				}
				// address in parsed into argument.Hex.Address
				if input.InternalType == "address" {
					if argument.Hex == nil {
						break
					}
					if argument.Hex.Address == nil {
						break
					}
				}
				// bytes32, array and tuples of any kind are only support via
				// hashes. Hashes are parsed into argument.Hex.String
				if input.InternalType == "bytes32" ||
					strings.Contains(input.InternalType, "[") ||
					strings.Contains(input.InternalType, "(") {
					if argument.Hex == nil {
						break
					}
					if argument.Hex.String == nil {
						break
					}
				}

				// we use strings.Contains here, because all integers are parsed into argument.Number
				if strings.Contains(input.InternalType, "int") && argument.Number == nil {
					break
				}

				compatible = true
			}

			if !compatible {
				suggestions = append(suggestions, function.Signature)
				continue
			}
		}

		return &function, nil, nil
	}

	return nil, suggestions, nil
}
