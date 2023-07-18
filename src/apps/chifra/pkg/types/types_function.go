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
	"io"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew"
	"github.com/ethereum/go-ethereum/accounts/abi"
)

// `payable` was present in ABIs before Solidity 0.5.0 and was replaced by `stateMutability`
// https://docs.soliditylang.org/en/develop/050-breaking-changes.html#command-line-and-json-interfaces
// EXISTING_CODE

type RawFunction struct {
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
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleFunction struct {
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
	raw             *RawFunction      `json:"-"`
	// EXISTING_CODE
	payable   bool
	abiMethod *abi.Method
	abiEvent  *abi.Event
	// EXISTING_CODE
}

func (s *SimpleFunction) Raw() *RawFunction {
	return s.raw
}

func (s *SimpleFunction) SetRaw(raw *RawFunction) {
	s.raw = raw
}

func (s *SimpleFunction) Model(verbose bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	if extraOptions["encodingSignatureOnly"] == true {
		return Model{
			Data: map[string]any{
				"encoding":  s.Encoding,
				"signature": s.Signature,
			},
			Order: []string{"encoding", "signature"},
		}
	}

	model = map[string]interface{}{
		"encoding":  s.Encoding,
		"name":      s.Name,
		"signature": s.Signature,
		"type":      s.FunctionType,
	}

	order = []string{
		"encoding",
		"type",
		"name",
		"signature",
	}

	if format == "json" {
		getParameterModels := func(params []SimpleParameter) []map[string]any {
			result := make([]map[string]any, len(params))
			for index, param := range params {
				result[index] = param.Model(verbose, format, extraOptions).Data
				result[index]["name"] = param.DisplayName(index)
			}
			return result
		}
		if verbose {
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

func (s *SimpleFunction) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleFunction) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
func (s *SimpleFunction) Clone() *SimpleFunction {
	shallowCopy := *s
	shallowCopy.Inputs = make([]SimpleParameter, len(s.Inputs))
	shallowCopy.Outputs = make([]SimpleParameter, len(s.Outputs))
	copy(shallowCopy.Inputs, s.Inputs)
	copy(shallowCopy.Outputs, s.Outputs)
	return &shallowCopy
}

func FunctionFromAbiEvent(ethEvent *abi.Event) *SimpleFunction {
	// ID is encoded signature
	encSig := strings.ToLower(ethEvent.ID.Hex())
	inputs := argumentsToSimpleParameters(ethEvent.Inputs)
	function := &SimpleFunction{
		Encoding:     encSig,
		Signature:    ethEvent.Sig,
		Name:         ethEvent.RawName,
		FunctionType: "event",
		Anonymous:    ethEvent.Anonymous,
		Inputs:       inputs,
	}
	function.SetAbiEvent(ethEvent)
	return function
}

// FunctionFromAbiMethod converts go-ethereum's abi.Method to our SimpleFunction
func FunctionFromAbiMethod(ethMethod *abi.Method) *SimpleFunction {
	// method.ID is our "four-byte"
	fourByte := "0x" + base.Bytes2Hex(ethMethod.ID)

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

// Pack encodes function call
func (s *SimpleFunction) Pack(callArguments []any) (packed []byte, err error) {
	abiMethod, err := s.GetAbiMethod()
	if err != nil {
		return
	}
	packedArgs, err := abiMethod.Inputs.Pack(callArguments...)
	if err != nil {
		return
	}
	packed = abiMethod.ID
	packed = append(packed, packedArgs...)

	return
}

func (s *SimpleFunction) MarshalCache(writer io.Writer) (err error) {
	if err = cacheNew.WriteValue(writer, s.Anonymous); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.Constant); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.Encoding); err != nil {
		return err
	}

	inputs := make([]cacheNew.Marshaler, 0, len(s.Inputs))
	for _, input := range s.Inputs {
		input := input
		inputs = append(inputs, &input)
	}
	if err = cacheNew.WriteValue(writer, inputs); err != nil {
		return err
	}

	if err = cacheNew.WriteValue(writer, s.Message); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.Name); err != nil {
		return err
	}

	outputs := make([]cacheNew.Marshaler, 0, len(s.Outputs))
	for _, output := range s.Outputs {
		outputs = append(outputs, &output)
	}
	if err = cacheNew.WriteValue(writer, outputs); err != nil {
		return err
	}

	if err = cacheNew.WriteValue(writer, s.Signature); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.StateMutability); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.FunctionType); err != nil {
		return err
	}

	return
}

func (s *SimpleFunction) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	if err = cacheNew.ReadValue(reader, &s.Anonymous, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.Constant, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.Encoding, version); err != nil {
		return err
	}

	s.Inputs = make([]SimpleParameter, 0)
	if err = cacheNew.ReadValue(reader, &s.Inputs, version); err != nil {
		return err
	}

	if err = cacheNew.ReadValue(reader, &s.Message, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.Name, version); err != nil {
		return err
	}

	s.Outputs = make([]SimpleParameter, 0)
	if err = cacheNew.ReadValue(reader, &s.Outputs, version); err != nil {
		return err
	}

	if err = cacheNew.ReadValue(reader, &s.Signature, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.StateMutability, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.FunctionType, version); err != nil {
		return err
	}

	return
}

// EXISTING_CODE
