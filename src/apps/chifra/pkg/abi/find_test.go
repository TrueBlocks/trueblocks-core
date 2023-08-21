package abi

import (
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/parser"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/accounts/abi"
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
var abis FunctionSyncMap
var packTestAbi abi.ABI

func init() {
	var err error
	testAbi, err = abi.JSON(strings.NewReader(abiSource))
	if err != nil {
		panic(err)
	}

	testHelperLoadAbisFromJson(&testAbi, &abis)

	packTestAbi, err = abi.JSON(strings.NewReader(packTestAbiSource))
	if err != nil {
		panic(err)
	}
}

func testHelperLoadAbisFromJson(parsedAbi *abi.ABI, destination *FunctionSyncMap) {
	for _, method := range parsedAbi.Methods {
		method := method
		encoding := "0x" + strings.ToLower(base.Bytes2Hex(method.ID))
		destination.SetValue(encoding, types.FunctionFromAbiMethod(&method))
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

	result, hints, err := FindAbiFunction(FindByName, call.Name, call.Arguments, &abis)
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

	result, hints, err = FindAbiFunction(FindByName, call.Name, call.Arguments, &abis)
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
	if hint != "setShouldReject(bool)" {
		t.Fatal("wrong hint:", hint)
	}

	// Expect no match
	call = &parser.FunctionContractCall{
		Name:      "somethingElse",
		Arguments: []*parser.ContractCallArgument{},
	}

	result, hints, err = FindAbiFunction(FindByName, call.Name, call.Arguments, &abis)
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

	result, hints, err := FindAbiFunction(FindBySelector, call.Selector.Value, call.Arguments, &abis)
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

	result, hints, err = FindAbiFunction(FindBySelector, call.Selector.Value, call.Arguments, &abis)
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
	if hint != "setShouldReject(bool)" {
		t.Fatal("wrong hint:", hint)
	}

	// Expect no match
	call = &parser.SelectorContractCall{
		Selector: parser.Selector{
			Value: "0xf175b639",
		},
		Arguments: []*parser.ContractCallArgument{},
	}

	result, hints, err = FindAbiFunction(FindBySelector, call.Selector.Value, call.Arguments, &abis)
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

func Test_findAbiFunctionMisleading(t *testing.T) {
	// This test makes sure we choose the right function
	// when given two very similar ones. In this case, it's
	// transfer(bytes32, address) and transfer(address, address).
	parsedAbi, err := abi.JSON(strings.NewReader(`
		[
			{ "type" : "function", "name" : "transfer", "inputs" : [ { "name" : "from", "type" : "bytes32" }, { "name" : "to", "type" : "address" } ] },
			{ "type" : "function", "name" : "transfer", "inputs" : [ { "name" : "from", "type" : "address" }, { "name" : "to", "type" : "address" } ] }
		]
	`))
	if err != nil {
		t.Fatal(err)
	}

	testHelperLoadAbisFromJson(&parsedAbi, &abis)

	call := &parser.FunctionContractCall{
		Name: "transfer",
		Arguments: []*parser.ContractCallArgument{
			{
				Hex: &parser.ContractCallHex{
					Address: utils.PointerOf(base.HexToAddress("0x6B175474E89094C44Da98b954EedeAC495271d0F")),
				},
			},
			{
				Hex: &parser.ContractCallHex{
					Address: utils.PointerOf(base.HexToAddress("0x6B175474E89094C44Da98b954EedeAC495271d0F")),
				},
			},
		},
	}

	result, hints, err := FindAbiFunction(FindByName, call.Name, call.Arguments, &abis)
	if err != nil {
		t.Fatal(err)
	}
	wanted := parsedAbi.Methods["transfer0"]
	if result == nil {
		t.Fatal("did not find anything")
	}
	if result.Signature != wanted.Sig {
		t.Fatal("found wrong function:", result.Signature)
	}
	if len(hints) > 0 {
		t.Fatal("expected no hints:", hints)
	}
}
