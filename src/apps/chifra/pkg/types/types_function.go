// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import (
	"strings"

	"github.com/ethereum/go-ethereum/accounts/abi"
	"github.com/ethereum/go-ethereum/common"
)

// EXISTING_CODE

type RawFunction struct {
	Outputs_dict    string `json:"outputs_dict"`
	Inputs_dict     string `json:"inputs_dict"`
	Abi_source      string `json:"abi_source"`
	Anonymous       string `json:"anonymous"`
	Constant        string `json:"constant"`
	StateMutability string `json:"stateMutability"`
	Message         string `json:"message"`
	Name            string `json:"name"`
	FunctionType    string `json:"type"`
	Signature       string `json:"signature"`
	Encoding        string `json:"encoding"`
	Inputs          string `json:"inputs"`
	Outputs         string `json:"outputs"`
}

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
	Message         string            `json:"message"`
	raw             *RawFunction
}

func (s *SimpleFunction) Raw() *RawFunction {
	return s.raw
}

func (s *SimpleFunction) SetRaw(raw *RawFunction) {
	s.raw = raw
}

func (s *SimpleFunction) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	// Used by chifra abis --find
	if extraOptions["encodingSignatureOnly"] == true {
		return Model{
			Data: map[string]any{
				"encoding":  s.Encoding,
				"signature": s.Signature,
			},
			Order: []string{"encoding", "signature"},
		}
	}
	// EXISTING_CODE

	model := map[string]interface{}{
		"name":            s.Name,
		"type":            s.FunctionType,
		"stateMutability": s.StateMutability,
		"signature":       s.Signature,
		"encoding":        s.Encoding,
	}

	order := []string{
		"encoding",
		"type",
		"name",
		"signature",
		"input_names",
		"output_names",
	}

	// EXISTING_CODE
	if format != "json" || extraOptions["verbose"] != true {
		model["input_names"] = joinParametersNames(s.Inputs)
		model["output_names"] = joinParametersNames(s.Outputs)
	}

	if format == "json" {
		getParameterModels := func(params []SimpleParameter) []map[string]any {
			result := make([]map[string]any, len(params))
			for index, param := range params {
				result[index] = param.Model(showHidden, format, extraOptions).Data
				result[index]["name"] = param.DisplayName(index)
			}
			return result
		}
		if extraOptions["verbose"] == true {
			model["inputs"] = getParameterModels(s.Inputs)
			model["outputs"] = getParameterModels(s.Outputs)
		}
		return Model{
			Data:  model,
			Order: []string{},
		}
	}

	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// argumentsToSimpleParameters converts slice of go-ethereum's Argument to slice of
// SimpleParameter
func argumentsToSimpleParameters(args []abi.Argument) (result []SimpleParameter) {
	result = make([]SimpleParameter, len(args))
	for index, arg := range args {
		result[index] = SimpleParameter{
			ParameterType: arg.Type.String(),
			Name:          arg.Name,
			Indexed:       arg.Indexed,
			InternalType:  getInternalType(&arg.Type),
			Components:    argumentTypesToSimpleParameters(arg.Type.TupleElems),
		}
	}
	return
}

func getInternalType(abiType *abi.Type) (result string) {
	result = abiType.TupleRawName
	if result == "" {
		result = abiType.String()
	}
	return
}

// argumentTypesToSimpleParameters is similar to argumentsToSimpleParameters, but for
// recursive types
func argumentTypesToSimpleParameters(argTypes []*abi.Type) (result []SimpleParameter) {
	result = make([]SimpleParameter, len(argTypes))
	for index, argType := range argTypes {
		result[index] = SimpleParameter{
			ParameterType: argType.String(),
			InternalType:  getInternalType(argType),
			Components:    argumentTypesToSimpleParameters(argType.TupleElems),
		}
	}
	return
}

func joinParametersNames(params []SimpleParameter) (result string) {
	for index, param := range params {
		if index > 0 {
			result += ","
		}
		result += param.DisplayName(index)
	}
	return
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

// FunctionFromAbiMethod converts go-ethereum's abi.Method to our SimpleFunction
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
	stateMutability := "nonpayable"
	if ethMethod.StateMutability != "" {
		stateMutability = ethMethod.StateMutability
	}
	return &SimpleFunction{
		Encoding:        fourByte,
		Signature:       ethMethod.Sig,
		Name:            ethMethod.Name,
		AbiSource:       abiSource,
		FunctionType:    functionType,
		Constant:        ethMethod.Constant,
		StateMutability: stateMutability,
		Inputs:          inputs,
		Outputs:         outputs,
	}
}

// EXISTING_CODE
