package types

import (
	"bytes"
	"encoding/json"
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
	// `payable` was present in ABIs before Solidity 0.5.0 and was replaced
	// by `stateMutability`: https://docs.soliditylang.org/en/develop/050-breaking-changes.html#command-line-and-json-interfaces
	payable   bool
	abiMethod *abi.Method
	abiEvent  *abi.Event
	// abiMethodEventMutex sync.Mutex
}

func FunctionFromAbiEvent(ethEvent *abi.Event, abiSource string) *SimpleFunction {
	// ID is encoded signature
	encSig := strings.ToLower(ethEvent.ID.Hex())
	inputs := argumentsToSimpleParameters(ethEvent.Inputs)
	function := &SimpleFunction{
		Encoding:     encSig,
		Signature:    ethEvent.Sig,
		Name:         ethEvent.Name,
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
	if ethMethod.StateMutability != "" {
		stateMutability = ethMethod.StateMutability
	}
	if ethMethod.Payable {
		stateMutability = "payable"
	}
	function := &SimpleFunction{
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

func FunctionToAbiMethod(function *SimpleFunction) (ethMethod *abi.Method, err error) {
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

func FunctionToAbiEvent(function *SimpleFunction) (ethMethod *abi.Event, err error) {
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

func (s *SimpleFunction) GetAbiMethod() *abi.Method {
	// if s.abiMethod == nil {
	// 	abiMethod, err := FunctionToAbiMethod(s)
	// 	// TODO(articulation): return error
	// 	if err != nil {
	// 		return nil
	// 	}
	// 	s.abiMethodEventMutex.Lock()
	// 	s.SetAbiMethod(abiMethod)
	// 	s.abiMethodEventMutex.Unlock()
	// 	return abiMethod
	// }
	return s.abiMethod
}

func (s *SimpleFunction) SetAbiEvent(event *abi.Event) {
	s.abiEvent = event
}

func (s *SimpleFunction) GetAbiEvent() *abi.Event {
	// if s.abiEvent == nil {
	// 	abiEvent, err := FunctionToAbiEvent(s)
	// 	// TODO(articulation): return error
	// 	if err != nil {
	// 		return nil
	// 	}
	// 	s.abiMethodEventMutex.Lock()
	// 	s.SetAbiEvent(abiEvent)
	// 	s.abiMethodEventMutex.Unlock()
	// 	return abiEvent
	// }
	return s.abiEvent
}

func (s *SimpleFunction) Raw() *RawFunction {
	return nil
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

func (s *SimpleFunction) Model(showHidden bool, format string, extraOptions map[string]any) Model {
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

	model := map[string]interface{}{
		"name":            s.Name,
		"type":            s.FunctionType,
		"stateMutability": s.StateMutability,
		"signature":       s.Signature,
		"encoding":        s.Encoding,
	}

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

	order := []string{
		"encoding",
		"type",
		"name",
		"signature",
		"input_names",
		"output_names",
	}

	return Model{
		Data:  model,
		Order: order,
	}
}

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
