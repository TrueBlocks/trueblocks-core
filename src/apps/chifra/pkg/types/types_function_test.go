package types

import (
	"reflect"
	"testing"

	"github.com/ethereum/go-ethereum/accounts/abi"
)

func TestFunctionToAbiMethod(t *testing.T) {
	input := &SimpleFunction{
		Name:         "deploy",
		FunctionType: "function",
		Inputs: []SimpleParameter{
			{
				Name:          "where",
				ParameterType: "address",
			},
			{
				Name:          "options",
				ParameterType: "tuple",
				InternalType:  "struct Options",
				Components: []SimpleParameter{
					{Name: "force", ParameterType: "bool"},
					{Name: "mode", ParameterType: "uint8"},
				},
			},
		},
	}
	result, err := FunctionToAbiMethod(input)
	if err != nil {
		t.Fatal(err)
	}

	if name := result.Name; name != "deploy" {
		t.Fatal("wrong name:", name)
	}

	if functiontype := result.Type; functiontype != abi.Function {
		t.Fatal("wrong functiontype:", functiontype)
	}

	if inputsLen := len(result.Inputs); inputsLen != 2 {
		t.Fatal("wrong inputs:", inputsLen)
	}

	if name := result.Inputs[0].Name; name != "where" {
		t.Fatal("wrong input 0 name:", name)
	}

	if inputType := result.Inputs[0].Type; inputType.T != abi.AddressTy {
		t.Fatal("wrong input 0 name:", inputType)
	}

	if name := result.Inputs[1].Name; name != "options" {
		t.Fatal("wrong input 1 name:", name)
	}

	if inputType := result.Inputs[1].Type; inputType.T != abi.TupleTy {
		t.Fatal("wrong input 1 name:", inputType)
	}

	if components := result.Inputs[1].Type.TupleRawNames; !reflect.DeepEqual(components, []string{"force", "mode"}) {
		t.Fatal("wrong input 1 components names:", components)
	}

	if componentType := result.Inputs[1].Type.TupleElems[0].T; componentType != abi.BoolTy {
		t.Fatal("wrong input 1 component 1 type:", componentType)
	}

	if componentType := result.Inputs[1].Type.TupleElems[1].T; componentType != abi.UintTy {
		t.Fatal("wrong input 1 component 2 type:", componentType)
	}
}
