package types

import (
	"fmt"
	"strings"

	"github.com/ethereum/go-ethereum/accounts/abi"
	"github.com/ethereum/go-ethereum/common"
)

type RawFunction interface{}

type SimpleFunction struct {
	Encoding        string            `json:"encoding,omitempty"`
	Signature       string            `json:"signature,omitempty"`
	Name            string            `json:"name"`
	FunctionType    string            `json:"type"`
	AbiSource       string            `json:"abi_source"`
	Anonymous       bool              `json:"anonymous"`
	Constant        bool              `json:"constant"`
	StateMutability string            `json:"stateMutability"`
	Inputs          []SimpleParameter `json:"inputs"`
	Outputs         []SimpleParameter `json:"outputs"`
}

func FunctionFromAbiEvent(ethEvent *abi.Event, abiSource string) *SimpleFunction {
	// ID is encoded signature
	encSig := strings.ToLower(ethEvent.ID.Hex())
	inputs := argumentsToSimpleParameters(ethEvent.Inputs)
	return &SimpleFunction{
		Encoding:     encSig,
		Signature:    ethEvent.Sig,
		Name:         ethEvent.Name,
		AbiSource:    abiSource,
		FunctionType: "event",
		Anonymous:    ethEvent.Anonymous,
		Inputs:       inputs,
	}
}

// TODO: abi.cpp:570 if ethMethod.Name == "" && ethMethod.Type != abi.Constructor { return }
// TODO: abi.cpp:580 if ethMethod.Type == abi.Constructor { return }
func FunctionFromAbiMethod(ethMethod *abi.Method, abiSource string) *SimpleFunction {
	// method.ID is our "four-byte"
	fourByte := "0x" + strings.ToLower(string(common.Bytes2Hex(ethMethod.ID)))

	var functionType string
	switch ethMethod.Type {
	case abi.Constructor:
		functionType = "constructor"
	case abi.Fallback:
		functionType = "fallback"
	case abi.Receive:
		functionType = "receive"
	default:
		functionType = "function"
	}

	inputs := argumentsToSimpleParameters(ethMethod.Inputs)
	outputs := argumentsToSimpleParameters(ethMethod.Outputs)
	return &SimpleFunction{
		Encoding:        fourByte,
		Signature:       ethMethod.Sig,
		Name:            ethMethod.Name,
		AbiSource:       abiSource,
		FunctionType:    functionType,
		Constant:        ethMethod.Constant,
		StateMutability: ethMethod.StateMutability,
		Inputs:          inputs,
		Outputs:         outputs,
	}
}

func argumentTypesToSimpleParameters(argTypes []*abi.Type) (result []SimpleParameter) {
	result = make([]SimpleParameter, len(argTypes))
	for index, argType := range argTypes {
		result[index] = SimpleParameter{
			ParameterType: argType.String(),
			InternalType:  argType.TupleRawName,
			Components:    argumentTypesToSimpleParameters(argType.TupleElems),
		}
	}
	return
}

func argumentsToSimpleParameters(args []abi.Argument) (result []SimpleParameter) {
	result = make([]SimpleParameter, len(args))
	for index, arg := range args {
		result[index] = SimpleParameter{
			ParameterType: arg.Type.String(),
			Name:          arg.Name,
			Indexed:       arg.Indexed,
			InternalType:  arg.Type.TupleRawName,
			Components:    argumentTypesToSimpleParameters(arg.Type.TupleElems),
		}
	}
	return
}

func (s *SimpleFunction) Raw() *RawFunction {
	return nil
}

func joinParametersNames(params []SimpleParameter) (result string) {
	for index, param := range params {
		if index > 0 {
			result += ","
		}
		var name string = param.Name
		if param.Name == "" {
			name = fmt.Sprintf("val_%d", index)
		}
		result += name
	}
	return
}

func (s *SimpleFunction) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	model := map[string]interface{}{
		"encoding":  s.Encoding,
		"type":      s.FunctionType,
		"name":      s.Name,
		"signature": s.Signature,
	}
	order := []string{
		"encoding",
		"type",
		"name",
		"signature",
	}

	if format != "json" {
		model["input_names"] = joinParametersNames(s.Inputs)
		model["output_names"] = joinParametersNames(s.Outputs)
		order = append(order, []string{
			"input_names",
			"output_names",
		}...)
	}

	return Model{
		Data:  model,
		Order: order,
	}
}

// TODO: remove this type when we move ABI output to StreamMany
type SimpleFunctionOutput struct {
	Encoding  string `json:"encoding,omitempty"`
	Signature string `json:"signature,omitempty"`
}
