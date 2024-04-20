package abi

import (
	"encoding/json"
	"os"
	"reflect"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func TestGenerateFromCombinedAst(t *testing.T) {
	fixture, err := os.OpenFile("./fixtures/fixture.json", os.O_RDONLY, 0)
	if err != nil {
		t.Fatal("error while opening fixture:", err)
	}

	result, err := GenerateFromCombinedAst(fixture)
	if err != nil {
		t.Fatal("error reading combined file:", err)
	}

	if rlen := len(result); rlen != 2 {
		t.Fatal("wrong result length", rlen)
	}

	j, _ := json.MarshalIndent(result, "", " ")
	os.WriteFile("/tmp/result.json", j, 0666)

	function := result[0]
	expected := types.Function{
		Encoding:        "52d1902d",
		Signature:       "proxiableUUID()",
		Name:            "proxiableUUID",
		FunctionType:    "function",
		Anonymous:       false,
		Constant:        false,
		StateMutability: "view",
		Inputs:          []types.Parameter{},
		Outputs: []types.Parameter{
			{
				ParameterType: "bytes32",
				Name:          "",
				Indexed:       false,
				InternalType:  "bytes32",
			},
		},
	}
	if !reflect.DeepEqual(function, expected) {
		t.Fatalf("got %v, expected %v", function, expected)
	}

	event := result[1]
	expected = types.Function{
		Encoding:        "7e644d79",
		Signature:       "AdminChanged(address,address)",
		Name:            "AdminChanged",
		FunctionType:    "event",
		Anonymous:       false,
		Constant:        false,
		StateMutability: "nonpayable",
		Inputs: []types.Parameter{
			{
				ParameterType: "address",
				Name:          "previousAdmin",
				Indexed:       false,
				InternalType:  "address",
			},
			{
				ParameterType: "address",
				Name:          "newAdmin",
				Indexed:       false,
				InternalType:  "address",
			},
		},
		Outputs: []types.Parameter{},
	}
	if !reflect.DeepEqual(event, expected) {
		t.Fatalf("got %v, expected %v", event, expected)
	}
}
