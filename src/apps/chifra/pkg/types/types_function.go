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
	"bytes"
	"encoding/json"
	"fmt"
	"strings"

	"github.com/ethereum/go-ethereum/accounts/abi"
	"github.com/ethereum/go-ethereum/common"
)

// EXISTING_CODE

type RawFunction struct {
	AbiSource       string `json:"abiSource"`
	Anonymous       string `json:"anonymous"`
	Constant        string `json:"constant"`
	Encoding        string `json:"encoding"`
	Inputs          string `json:"inputs"`
	Message         string `json:"message"`
	Name            string `json:"name"`
	Outputs         string `json:"outputs"`
	Signature       string `json:"signature"`
	StateMutability string `json:"stateMutability"`
	FunctionType    string `json:"type"`
}

type SimpleFunction struct {
	AbiSource       string            `json:"abiSource,omitempty"`
	Anonymous       bool              `json:"anonymous,omitempty"`
	Constant        bool              `json:"constant,omitempty"`
	Encoding        string            `json:"encoding"`
	Inputs          []SimpleParameter `json:"inputs"`
	Message         string            `json:"message,omitempty"`
	Name            string            `json:"name"`
	Outputs         []SimpleParameter `json:"outputs"`
	Signature       string            `json:"signature,omitempty"`
	StateMutability string            `json:"stateMutability,omitempty"`
	FunctionType    string            `json:"type"`
	// `payable` was present in ABIs before Solidity 0.5.0 and was replaced
	// by `stateMutability`: https://docs.soliditylang.org/en/develop/050-breaking-changes.html#command-line-and-json-interfaces
	payable   bool
	abiMethod *abi.Method
	abiEvent  *abi.Event
	raw       *RawFunction
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
		"encoding":  s.Encoding,
		"name":      s.Name,
		"signature": s.Signature,
		"type":      s.FunctionType,
	}

	order := []string{
		"encoding",
		"type",
		"name",
		"signature",
	}

	// EXISTING_CODE
	// re-ordering
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
			inputs := getParameterModels(s.Inputs)
			if inputs != nil {
				model["inputs"] = inputs
			}
			outputs := getParameterModels(s.Outputs)
			if outputs != nil {
				model["outputs"] = outputs
			}
		}
		sm := s.StateMutability
		if sm != "" && sm != "nonpayable" && sm != "view" {
			model["stateMutability"] = sm
		}
	}

	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
func FunctionFromAbiEvent(ethEvent *abi.Event, abiSource string) *SimpleFunction {
	// ID is encoded signature
	encSig := strings.ToLower(ethEvent.ID.Hex())
	inputs := argumentsToSimpleParameters(ethEvent.Inputs)
	function := &SimpleFunction{
		Encoding:     encSig,
		Signature:    ethEvent.Sig,
		Name:         ethEvent.RawName,
		AbiSource:    abiSource,
		FunctionType: "event",
		Anonymous:    ethEvent.Anonymous,
		Inputs:       inputs,
	}
	function.SetAbiEvent(ethEvent)
	return function
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
	if ethMethod.StateMutability != "" && ethMethod.StateMutability != "nonpayable" && ethMethod.StateMutability != "view" {
		stateMutability = ethMethod.StateMutability
	} else if ethMethod.Payable {
		stateMutability = "payable"
	}
	function := &SimpleFunction{
		Encoding:        fourByte,
		Signature:       ethMethod.Sig,
		Name:            ethMethod.RawName,
		AbiSource:       abiSource,
		FunctionType:    functionType,
		Constant:        ethMethod.Constant,
		StateMutability: stateMutability,
		Inputs:          inputs,
		Outputs:         outputs,
	}
	function.SetAbiMethod(ethMethod)
	return function
}

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

func AbiMethodFromFunction(function *SimpleFunction) (ethMethod *abi.Method, err error) {
	if !function.IsMethod() {
		err = fmt.Errorf("FunctionToAbiMethod called for an event")
		return
	}

	removeUnknownTuples(function)
	jsonAbi, err := json.Marshal([]any{function})
	if err != nil {
		return
	}
	res, err := abi.JSON(bytes.NewReader(jsonAbi))
	if err != nil {
		return
	}
	found, ok := res.Methods[function.Name]
	if !ok {
		err = fmt.Errorf("generating ABI method: method not found: %s", function.Name)
		return
	}
	ethMethod = &found
	return
}

func AbiEventFromFunction(function *SimpleFunction) (ethMethod *abi.Event, err error) {
	if function.IsMethod() {
		err = fmt.Errorf("functionToAbiEvent called for a method")
		return
	}

	removeUnknownTuples(function)
	jsonAbi, err := json.Marshal([]any{function})
	if err != nil {
		return
	}
	res, err := abi.JSON(bytes.NewReader(jsonAbi))
	if err != nil {
		return
	}
	found, ok := res.Events[function.Name]
	if !ok {
		err = fmt.Errorf("generating ABI method: method not found: %s", function.Name)
		return
	}
	ethMethod = &found
	return
}

// removeUnknownTuples replaces unknown tuple type with `bytes` type.
// A tuple is unknown if we don't know its components (this can happen for
// inputs/outputs of internal methods)
func removeUnknownTuples(function *SimpleFunction) {
	remove := func(params []SimpleParameter) {
		for i := 0; i < len(params); i++ {
			param := params[i]
			parameterType := param.ParameterType
			// Unknown struct
			if parameterType == "()" || parameterType == "()[]" {
				params[i].ParameterType = "bytes"
			}
		}
	}

	remove(function.Inputs)
	remove(function.Outputs)
}

func (s *SimpleFunction) IsMethod() bool {
	return s.FunctionType != "event"
}

func (s *SimpleFunction) SetAbiMethod(method *abi.Method) {
	s.abiMethod = method
}

func (s *SimpleFunction) GetAbiMethod() (abiMethod *abi.Method, err error) {
	if s.abiMethod == nil {
		abiMethod, err = AbiMethodFromFunction(s)
		if err != nil {
			return
		}
		s.SetAbiMethod(abiMethod)
		return
	}
	return s.abiMethod, nil
}

func (s *SimpleFunction) SetAbiEvent(event *abi.Event) {
	s.abiEvent = event
}

func (s *SimpleFunction) GetAbiEvent() (abiEvent *abi.Event, err error) {
	if s.abiEvent == nil {
		abiEvent, err = AbiEventFromFunction(s)
		if err != nil {
			return
		}
		s.SetAbiEvent(abiEvent)
		return
	}
	return s.abiEvent, nil
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

// TODO: I feel like we might be able to remove stateMutability since we don't really use it.
// Normalize sets StateMutability from `payable` field. It is only useful when
// reading ABIs generated before Solidity 0.5.0, which use `payable` field:
// https://docs.soliditylang.org/en/develop/050-breaking-changes.html#command-line-and-json-interfaces
func (s *SimpleFunction) Normalize() {
	// if StateMutability is already set, we don't have to do anything
	if s.StateMutability != "nonpayable" {
		return
	}
	if s.payable {
		s.StateMutability = "payable"
		return
	}
	s.StateMutability = "nonpayable"
}

// EXISTING_CODE
