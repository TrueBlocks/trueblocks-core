// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/ethereum/go-ethereum/accounts/abi"
)

// `payable` was present in ABIs before Solidity 0.5.0 and was replaced by `stateMutability`
// https://docs.soliditylang.org/en/develop/050-breaking-changes.html#command-line-and-json-interfaces
// EXISTING_CODE

type Function struct {
	Anonymous       bool        `json:"anonymous,omitempty"`
	Constant        bool        `json:"constant,omitempty"`
	Encoding        string      `json:"encoding"`
	Inputs          []Parameter `json:"inputs"`
	Message         string      `json:"message,omitempty"`
	Name            string      `json:"name"`
	Outputs         []Parameter `json:"outputs"`
	Signature       string      `json:"signature,omitempty"`
	StateMutability string      `json:"stateMutability,omitempty"`
	FunctionType    string      `json:"type"`
	// EXISTING_CODE
	payable   bool
	abiMethod *abi.Method
	abiEvent  *abi.Event
	// EXISTING_CODE
}

func (s Function) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Function) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	if extraOpts["encodingSignatureOnly"] == true {
		return Model{
			Data: map[string]any{
				"encoding":  s.Encoding,
				"signature": s.Signature,
			},
			Order: []string{"encoding", "signature"},
		}
	}

	model = map[string]any{
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
		getParameterModels := func(params []Parameter) []map[string]any {
			result := make([]map[string]any, len(params))
			for index, param := range params {
				result[index] = param.Model(chain, format, verbose, extraOpts).Data
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

func (s *Function) MarshalCache(writer io.Writer) (err error) {
	// Anonymous
	if err = cache.WriteValue(writer, s.Anonymous); err != nil {
		return err
	}

	// Constant
	if err = cache.WriteValue(writer, s.Constant); err != nil {
		return err
	}

	// Encoding
	if err = cache.WriteValue(writer, s.Encoding); err != nil {
		return err
	}

	// Inputs
	inputs := make([]cache.Marshaler, 0, len(s.Inputs))
	for _, input := range s.Inputs {
		inputs = append(inputs, &input)
	}
	if err = cache.WriteValue(writer, inputs); err != nil {
		return err
	}

	// Message
	if err = cache.WriteValue(writer, s.Message); err != nil {
		return err
	}

	// Name
	if err = cache.WriteValue(writer, s.Name); err != nil {
		return err
	}

	// Outputs
	outputs := make([]cache.Marshaler, 0, len(s.Outputs))
	for _, output := range s.Outputs {
		outputs = append(outputs, &output)
	}
	if err = cache.WriteValue(writer, outputs); err != nil {
		return err
	}

	// Signature
	if err = cache.WriteValue(writer, s.Signature); err != nil {
		return err
	}

	// StateMutability
	if err = cache.WriteValue(writer, s.StateMutability); err != nil {
		return err
	}

	// FunctionType
	if err = cache.WriteValue(writer, s.FunctionType); err != nil {
		return err
	}

	return nil
}

func (s *Function) UnmarshalCache(vers uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// Anonymous
	if err = cache.ReadValue(reader, &s.Anonymous, vers); err != nil {
		return err
	}

	// Constant
	if err = cache.ReadValue(reader, &s.Constant, vers); err != nil {
		return err
	}

	// Encoding
	if err = cache.ReadValue(reader, &s.Encoding, vers); err != nil {
		return err
	}

	// Inputs
	s.Inputs = make([]Parameter, 0)
	if err = cache.ReadValue(reader, &s.Inputs, vers); err != nil {
		return err
	}

	// Message
	if err = cache.ReadValue(reader, &s.Message, vers); err != nil {
		return err
	}

	// Name
	if err = cache.ReadValue(reader, &s.Name, vers); err != nil {
		return err
	}

	// Outputs
	s.Outputs = make([]Parameter, 0)
	if err = cache.ReadValue(reader, &s.Outputs, vers); err != nil {
		return err
	}

	// Signature
	if err = cache.ReadValue(reader, &s.Signature, vers); err != nil {
		return err
	}

	// StateMutability
	if err = cache.ReadValue(reader, &s.StateMutability, vers); err != nil {
		return err
	}

	// FunctionType
	if err = cache.ReadValue(reader, &s.FunctionType, vers); err != nil {
		return err
	}

	s.FinishUnmarshal()

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Function) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
//

func (s *Function) Clone() *Function {
	shallowCopy := *s
	shallowCopy.Inputs = make([]Parameter, len(s.Inputs))
	shallowCopy.Outputs = make([]Parameter, len(s.Outputs))
	copy(shallowCopy.Inputs, s.Inputs)
	copy(shallowCopy.Outputs, s.Outputs)
	return &shallowCopy
}

func FunctionFromAbiEvent(ethEvent *abi.Event) *Function {
	// ID is encoded signature
	encSig := strings.ToLower(ethEvent.ID.Hex())
	inputs := argumentsToParameters(ethEvent.Inputs)
	function := &Function{
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

// FunctionFromAbiMethod converts go-ethereum's abi.Method to our Function
func FunctionFromAbiMethod(ethMethod *abi.Method) *Function {
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

	inputs := argumentsToParameters(ethMethod.Inputs)
	outputs := argumentsToParameters(ethMethod.Outputs)
	stateMutability := "nonpayable"
	if ethMethod.StateMutability != "" && ethMethod.StateMutability != "nonpayable" && ethMethod.StateMutability != "view" {
		stateMutability = ethMethod.StateMutability
	} else if ethMethod.Payable {
		stateMutability = "payable"
	}
	function := &Function{
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

// argumentsToParameters converts slice of go-ethereum's Argument to slice of
// Parameter
func argumentsToParameters(args []abi.Argument) (result []Parameter) {
	result = make([]Parameter, len(args))
	for index, arg := range args {
		result[index] = Parameter{
			ParameterType: arg.Type.String(),
			Name:          arg.Name,
			Indexed:       arg.Indexed,
			InternalType:  getInternalType(&arg.Type),
			Components:    argumentTypesToParameters(arg.Type.TupleElems),
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

// argumentTypesToParameters is similar to argumentsToParameters, but for
// recursive types
func argumentTypesToParameters(argTypes []*abi.Type) (result []Parameter) {
	result = make([]Parameter, len(argTypes))
	for index, argType := range argTypes {
		result[index] = Parameter{
			ParameterType: argType.String(),
			InternalType:  getInternalType(argType),
			Components:    argumentTypesToParameters(argType.TupleElems),
		}
	}
	return
}

func (s *Function) AbiMethodFromFunction() (ethMethod *abi.Method, err error) {
	if !s.IsMethod() {
		err = fmt.Errorf("FunctionToAbiMethod called for an event")
		return
	}

	removeUnknownTuples(s)
	jsonAbi, err := json.Marshal([]any{s})
	if err != nil {
		return
	}
	res, err := abi.JSON(bytes.NewReader(jsonAbi))
	if err != nil {
		return
	}
	found, ok := res.Methods[s.Name]
	if !ok {
		err = fmt.Errorf("generating ABI method: method not found: %s", s.Name)
		return
	}
	ethMethod = &found
	return
}

func (s *Function) AbiEventFromFunction() (ethMethod *abi.Event, err error) {
	if s.IsMethod() {
		err = fmt.Errorf("functionToAbiEvent called for a method")
		return
	}

	removeUnknownTuples(s)
	jsonAbi, err := json.Marshal([]any{s})
	if err != nil {
		return
	}
	res, err := abi.JSON(bytes.NewReader(jsonAbi))
	if err != nil {
		return
	}
	found, ok := res.Events[s.Name]
	if !ok {
		err = fmt.Errorf("generating ABI method: method not found: %s", s.Name)
		return
	}
	ethMethod = &found
	return
}

// removeUnknownTuples replaces unknown tuple type with `bytes` type.
// A tuple is unknown if we don't know its components (this can happen for
// inputs/outputs of internal methods)
func removeUnknownTuples(function *Function) {
	remove := func(params []Parameter) {
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

func (s *Function) IsMethod() bool {
	return s.FunctionType != "event"
}

func (s *Function) SetAbiMethod(method *abi.Method) {
	s.abiMethod = method
}

func (s *Function) GetAbiMethod() (abiMethod *abi.Method, err error) {
	if s.abiMethod == nil {
		abiMethod, err = s.AbiMethodFromFunction()
		if err != nil {
			return
		}
		s.SetAbiMethod(abiMethod)
		return
	}
	return s.abiMethod, nil
}

func (s *Function) SetAbiEvent(event *abi.Event) {
	s.abiEvent = event
}

func (s *Function) GetAbiEvent() (abiEvent *abi.Event, err error) {
	if s.abiEvent == nil {
		abiEvent, err = s.AbiEventFromFunction()
		if err != nil {
			return
		}
		s.SetAbiEvent(abiEvent)
		return
	}
	return s.abiEvent, nil
}

// Normalize sets StateMutability from `payable` field. It is only useful when
// reading ABIs generated before Solidity 0.5.0, which use `payable` field:
// https://docs.soliditylang.org/en/develop/050-breaking-changes.html#command-line-and-json-interfaces
func (s *Function) Normalize() {
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
func (s *Function) Pack(callArguments []any) (packed []byte, err error) {
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

// EXISTING_CODE
