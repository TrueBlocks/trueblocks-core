package abi

import (
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/parser"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/accounts/abi"
	"github.com/ethereum/go-ethereum/common"
)

var abiSource = `
[
  {
    "encoding": "0xa175b638",
    "inputs": [
      {
        "name": "_value",
        "type": "bool"
      }
    ],
    "name": "setShouldReject",
    "outputs": [],
    "signature": "setShouldReject(bool)",
    "type": "function"
  },
  {
    "constant": true,
    "encoding": "0x01ffc9a7",
    "inputs": [
      {
        "name": "interfaceID",
        "type": "bytes4"
      }
    ],
    "name": "supportsInterface",
    "outputs": [
      {
        "name": "ret_0",
        "type": "bool"
      }
    ],
    "signature": "supportsInterface(bytes4)",
    "type": "function"
  },
  {
    "constant": true,
    "encoding": "0x0e89341c",
    "inputs": [
      {
        "name": "_id",
        "type": "uint256"
      }
    ],
    "name": "uri",
    "outputs": [
      {
        "name": "memory",
        "type": "string"
      }
    ],
    "signature": "uri(uint256)",
    "type": "function"
  }
]
`

// This comes from go-ethereum's abi_test.go, we will use it to
// test packing
const packTestAbiSource = `
[
	{ "type" : "function", "name" : ""},
	{ "type" : "function", "name" : "string", "inputs" : [ { "name" : "inputs", "type" : "string" } ] },
	{ "type" : "function", "name" : "bool", "inputs" : [ { "name" : "inputs", "type" : "bool" } ] },
	{ "type" : "function", "name" : "address", "inputs" : [ { "name" : "inputs", "type" : "address" } ] },
	{ "type" : "function", "name" : "uint64", "inputs" : [ { "name" : "inputs", "type" : "uint64" } ] },
	{ "type" : "function", "name" : "uint256", "inputs" : [ { "name" : "inputs", "type" : "uint256" } ] },
	{ "type" : "function", "name" : "int8", "inputs" : [ { "name" : "inputs", "type" : "int8" } ] },
	{ "type" : "function", "name" : "int64", "inputs" : [ { "name" : "inputs", "type" : "int64" } ] },
	{ "type" : "function", "name" : "int256", "inputs" : [ { "name" : "inputs", "type" : "int256" } ] },
	{ "type" : "function", "name" : "bytes4", "inputs" : [ { "name" : "inputs", "type" : "bytes4" } ] },
	{ "type" : "function", "name" : "bytes32", "inputs" : [ { "name" : "inputs", "type" : "bytes32" } ] }
]`

var testAbi abi.ABI
var abis map[string]*types.SimpleFunction
var packTestAbi abi.ABI

func init() {
	var err error
	testAbi, err = abi.JSON(strings.NewReader(abiSource))
	if err != nil {
		panic(err)
	}

	abis = make(AbiInterfaceMap, len(testAbi.Methods))
	for _, method := range testAbi.Methods {
		method := method
		encoding := "0x" + strings.ToLower(common.Bytes2Hex(method.ID))
		abis[encoding] = types.FunctionFromAbiMethod(&method)
	}

	packTestAbi, err = abi.JSON(strings.NewReader(packTestAbiSource))
	if err != nil {
		panic(err)
	}
}

func Test_findAbiFunction(t *testing.T) {
	call := &parser.FunctionContractCall{
		Name: "setShouldReject",
		Arguments: []*parser.ContractCallArgument{
			{
				Boolean: utils.PointerOf(parser.Boolean(true)),
			},
		},
	}

	result, hints, err := FindAbiFunction(FindByName, call.Name, call.Arguments, abis)
	if err != nil {
		t.Fatal(err)
	}
	wanted := testAbi.Methods["setShouldReject"]

	if result.Signature != wanted.Sig {
		t.Fatal("wanted", wanted.Sig, "got", result.Signature)
	}

	if len(hints) > 0 {
		t.Fatal("expected no hints")
	}

	// Expect hints
	call = &parser.FunctionContractCall{
		Name:      "setShouldReject",
		Arguments: []*parser.ContractCallArgument{},
	}

	result, hints, err = FindAbiFunction(FindByName, call.Name, call.Arguments, abis)
	if err != nil {
		t.Fatal(err)
	}

	if result != nil {
		t.Fatal("expected no result")
	}

	if len(hints) != 1 {
		t.Fatal("expected single hint", hints)
	}

	hint := hints[0]
	if hint.Name != "setShouldReject" {
		t.Fatal("wrong hint:", hint.Name)
	}

	// Expect no match
	call = &parser.FunctionContractCall{
		Name:      "somethingElse",
		Arguments: []*parser.ContractCallArgument{},
	}

	result, hints, err = FindAbiFunction(FindByName, call.Name, call.Arguments, abis)
	if err != nil {
		t.Fatal(err)
	}

	if result != nil {
		t.Fatal("expected no result")
	}

	if len(hints) != 0 {
		t.Fatal("expected no hints")
	}
}

func Test_findAbiFunctionBySelector(t *testing.T) {
	call := &parser.SelectorContractCall{
		Selector: parser.Selector{
			Value: "0xa175b638",
		},
		Arguments: []*parser.ContractCallArgument{
			{
				Boolean: utils.PointerOf(parser.Boolean(true)),
			},
		},
	}

	result, hints, err := FindAbiFunction(FindBySelector, call.Selector.Value, call.Arguments, abis)
	if err != nil {
		t.Fatal(err)
	}
	wanted := testAbi.Methods["setShouldReject"]

	if result.Signature != wanted.Sig {
		t.Fatal("wanted", wanted.Sig, "got", result.Signature)
	}

	if len(hints) > 0 {
		t.Fatal("expected no hints")
	}

	// Expect hints
	call = &parser.SelectorContractCall{
		Selector: parser.Selector{
			Value: "0xa175b638",
		},
		Arguments: []*parser.ContractCallArgument{},
	}

	result, hints, err = FindAbiFunction(FindBySelector, call.Selector.Value, call.Arguments, abis)
	if err != nil {
		t.Fatal(err)
	}

	if result != nil {
		t.Fatal("expected no result")
	}

	if len(hints) != 1 {
		t.Fatal("expected single hint", hints)
	}

	hint := hints[0]
	if hint.Name != "setShouldReject" {
		t.Fatal("wrong hint:", hint.Name)
	}

	// Expect no match
	call = &parser.SelectorContractCall{
		Selector: parser.Selector{
			Value: "0xf175b639",
		},
		Arguments: []*parser.ContractCallArgument{},
	}

	result, hints, err = FindAbiFunction(FindBySelector, call.Selector.Value, call.Arguments, abis)
	if err != nil {
		t.Fatal(err)
	}

	if result != nil {
		t.Fatal("expected no result")
	}

	if len(hints) != 0 {
		t.Fatal("expected no hints")
	}
}
