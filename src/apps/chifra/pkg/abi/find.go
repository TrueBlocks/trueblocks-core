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

// FindAbiFunction returns either the function to call or a list of suggestions (functions
// with the same name, but different argument count)
func FindAbiFunction(mode FindMode, identifier string, arguments []*parser.ContractCallArgument, abis AbiInterfaceMap) (fn *types.SimpleFunction, suggestions []types.SimpleFunction, err error) {
	for _, function := range abis {
		function := function
		// TODO: is this too naive?
		if mode == FindByName && function.Name != identifier {
			continue
		}
		if mode == FindBySelector && function.Encoding != strings.ToLower(identifier) {
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
