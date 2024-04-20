package abi

import (
	"encoding/json"
	"fmt"
	"io"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/crypto"
)

type SolcCombinedFile struct {
	// Sources map[string]struct {
	AST SolcAst `json:"ast"`
	// } `json:"sources"`
}

type SolcAst struct {
	Nodes []solcNode `json:"nodes"`
}

// no signature!
type solcNode struct {
	Anonymous        bool              `json:"anonymous"`
	FunctionSelector string            `json:"functionSelector"`
	NodeType         string            `json:"nodeType"` // FunctionDefinition, EventDefinition
	Kind             string            `json:"kind"`
	Name             string            `json:"name"`
	StateMutability  string            `json:"stateMutability"`
	Parameters       solcParameterList `json:"parameters"`
	ReturnParameters solcParameterList `json:"returnParameters"`
	Nodes            []solcNode        `json:"nodes"`
}

type solcParameterList struct {
	Parameters []solcNodeParameters `json:"parameters"`
}

type solcNodeParameters struct {
	Constant bool   `json:"contant"`
	Indexed  bool   `json:"indexed"`
	Name     string `json:"name"`
	TypeName struct {
		Name string
	} `json:"typeName"`
}

func GenerateFromCombinedAst(reader io.Reader) (result []types.Function, err error) {
	decoder := json.NewDecoder(reader)
	cf := &SolcCombinedFile{}
	if err = decoder.Decode(cf); err != nil {
		return
	}
	return fromCombinedAst(cf), nil
}

func fromCombinedAst(cf *SolcCombinedFile) (result []types.Function) {
	// for _, entry := range cf.Sources {
	for _, node := range cf.AST.Nodes {
		result = append(result, visitNodes(&node)...)
	}
	// }
	return
}

func visitNodes(node *solcNode) (result []types.Function) {
	switch node.NodeType {
	case "FunctionDefinition":
		fallthrough
	case "EventDefinition":
		inputs := make([]string, len(node.Parameters.Parameters))
		inputParams := make([]types.Parameter, len(node.Parameters.Parameters))
		for index, input := range node.Parameters.Parameters {
			inputs[index] = input.TypeName.Name
			inputParams[index] = types.Parameter{
				ParameterType: input.TypeName.Name,
				Name:          input.Name,
				Indexed:       input.Indexed,
				// TODO: is this correct?
				InternalType: input.TypeName.Name,
				// Components    []Parameter `json:"components"`
			}
		}
		outputParams := make([]types.Parameter, len(node.ReturnParameters.Parameters))
		for index, output := range node.ReturnParameters.Parameters {
			outputParams[index] = types.Parameter{
				ParameterType: output.TypeName.Name,
				Name:          output.Name,
				Indexed:       output.Indexed,
				// TODO: is this correct?
				InternalType: output.TypeName.Name,
				// Components    []Parameter `json:"components"`
			}
		}

		sig := fmt.Sprintf("%v(%v)", node.Name, strings.Join(inputs, ","))
		// four-byte
		selector := crypto.Keccak256([]byte(sig))[:4]
		var stateMutability string
		if node.StateMutability != "" {
			stateMutability = node.StateMutability
		} else {
			stateMutability = "nonpayable"
		}

		var fnType string
		if node.Kind != "" {
			fnType = node.Kind
		} else {
			fnType = "event"
		}

		result = append(result, types.Function{
			Encoding:        base.Bytes2Hex(selector),
			Signature:       sig,
			Name:            node.Name,
			FunctionType:    fnType,
			Anonymous:       node.Anonymous,
			Constant:        false,
			StateMutability: stateMutability,
			Inputs:          inputParams,
			Outputs:         outputParams,
		})
	}

	if len(node.Nodes) > 0 {
		for _, child := range node.Nodes {
			childResult := visitNodes(&child)
			result = append(result, childResult...)
		}
	}
	return
}
