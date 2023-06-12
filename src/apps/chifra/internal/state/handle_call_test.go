package statePkg

import (
	"math/big"
	"reflect"
	"strings"
	"testing"

	abiPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
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

	abis = make(abiPkg.AbiInterfaceMap, len(testAbi.Methods))
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
	call := &FunctionCall{
		Name: "setShouldReject",
		Arguments: []*Argument{
			{
				Boolean: utils.PointerOf(Boolean(true)),
			},
		},
	}

	result, hints, err := findAbiFunction(findByName, call.Name, call.Arguments, abis)
	if err != nil {
		t.Fatal(err)
	}
	wanted := testAbi.Methods["setShouldReject"]

	if result.Sig != wanted.Sig {
		t.Fatal("wanted", wanted.Sig, "got", result.Sig)
	}

	if len(hints) > 0 {
		t.Fatal("expected no hints")
	}

	// Expect hints
	call = &FunctionCall{
		Name:      "setShouldReject",
		Arguments: []*Argument{},
	}

	result, hints, err = findAbiFunction(findByName, call.Name, call.Arguments, abis)
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
	call = &FunctionCall{
		Name:      "somethingElse",
		Arguments: []*Argument{},
	}

	result, hints, err = findAbiFunction(findByName, call.Name, call.Arguments, abis)
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
	call := &SelectorCall{
		Selector: Selector{
			Value: "0xa175b638",
		},
		Arguments: []*Argument{
			{
				Boolean: utils.PointerOf(Boolean(true)),
			},
		},
	}

	result, hints, err := findAbiFunction(findBySelector, call.Selector.Value, call.Arguments, abis)
	if err != nil {
		t.Fatal(err)
	}
	wanted := testAbi.Methods["setShouldReject"]

	if result.Sig != wanted.Sig {
		t.Fatal("wanted", wanted.Sig, "got", result.Sig)
	}

	if len(hints) > 0 {
		t.Fatal("expected no hints")
	}

	// Expect hints
	call = &SelectorCall{
		Selector: Selector{
			Value: "0xa175b638",
		},
		Arguments: []*Argument{},
	}

	result, hints, err = findAbiFunction(findBySelector, call.Selector.Value, call.Arguments, abis)
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
	call = &SelectorCall{
		Selector: Selector{
			Value: "0xf175b639",
		},
		Arguments: []*Argument{},
	}

	result, hints, err = findAbiFunction(findBySelector, call.Selector.Value, call.Arguments, abis)
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

func Test_packFunction(t *testing.T) {
	int256 := big.NewInt(0)
	_, ok := int256.SetString("-57896044618658097711785492504343953926634992332820282019728792003956564819968", 0)
	if !ok {
		t.Fatal("cannot set int256 value")
	}

	uint256 := big.NewInt(0)
	_, ok = uint256.SetString("115792089237316195423570985008687907853269984665640564039457584007913129639935", 0)
	if !ok {
		t.Fatal("cannot set uint256 value")
	}

	type args struct {
		call     *FunctionCall
		function *abi.Method
	}
	tests := []struct {
		name       string
		args       args
		wantPacked []byte
		wantErr    bool
	}{
		{
			name: "string argument",
			args: args{
				call: &FunctionCall{
					Name: "string",
					Arguments: []*Argument{
						{
							String: utils.PointerOf("hello"),
						},
					},
				},
				function: utils.PointerOf(packTestAbi.Methods["string"]),
			},
			wantPacked: common.Hex2Bytes("3fcf74c60000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000000568656c6c6f000000000000000000000000000000000000000000000000000000"),
		},
		{
			name: "boolean argument",
			args: args{
				call: &FunctionCall{
					Name: "bool",
					Arguments: []*Argument{
						{
							Boolean: utils.PointerOf(Boolean(true)),
						},
					},
				},
				function: utils.PointerOf(packTestAbi.Methods["bool"]),
			},
			wantPacked: common.Hex2Bytes("17192efc0000000000000000000000000000000000000000000000000000000000000001"),
		},
		{
			name: "address argument",
			args: args{
				call: &FunctionCall{
					Name: "address",
					Arguments: []*Argument{
						{
							Hex: &HexValue{
								Address: utils.PointerOf(base.HexToAddress("0xE2e87901a3E1CB22b9853AE0827DE34E6Fae0575")),
							},
						},
					},
				},
				function: utils.PointerOf(packTestAbi.Methods["address"]),
			},
			wantPacked: common.Hex2Bytes("0706105e000000000000000000000000e2e87901a3e1cb22b9853ae0827de34e6fae0575"),
		},
		{
			name: "uint64 argument",
			args: args{
				call: &FunctionCall{
					Name: "uint64",
					Arguments: []*Argument{
						{
							Number: &Number{
								Uint: utils.PointerOf(uint64(18446744073709551615)),
							},
						},
					},
				},
				function: utils.PointerOf(packTestAbi.Methods["uint64"]),
			},
			wantPacked: common.Hex2Bytes("8ab008da000000000000000000000000000000000000000000000000ffffffffffffffff"),
		},
		{
			name: "uint256 argument",
			args: args{
				call: &FunctionCall{
					Name: "uint256",
					Arguments: []*Argument{
						{
							Number: &Number{
								Big: uint256,
							},
						},
					},
				},
				function: utils.PointerOf(packTestAbi.Methods["uint256"]),
			},
			wantPacked: common.Hex2Bytes("6449cd53ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"),
		},
		{
			name: "int8 argument",
			args: args{
				call: &FunctionCall{
					Name: "int8",
					Arguments: []*Argument{
						{
							Number: &Number{
								Int: utils.PointerOf(int64(-128)),
							},
						},
					},
				},
				function: utils.PointerOf(packTestAbi.Methods["int8"]),
			},
			wantPacked: common.Hex2Bytes("22aca450ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff80"),
		},
		{
			name: "int64 argument",
			args: args{
				call: &FunctionCall{
					Name: "int64",
					Arguments: []*Argument{
						{
							Number: &Number{
								Int: utils.PointerOf(int64(-9223372036854775808)),
							},
						},
					},
				},
				function: utils.PointerOf(packTestAbi.Methods["int64"]),
			},
			wantPacked: common.Hex2Bytes("4949bf43ffffffffffffffffffffffffffffffffffffffffffffffff8000000000000000"),
		},
		{
			name: "int256 argument",
			args: args{
				call: &FunctionCall{
					Name: "int256",
					Arguments: []*Argument{
						{
							Number: &Number{
								Big: int256,
							},
						},
					},
				},
				function: utils.PointerOf(packTestAbi.Methods["int256"]),
			},
			wantPacked: common.Hex2Bytes("4280896d8000000000000000000000000000000000000000000000000000000000000000"),
		},
		{
			name: "fixed-bytes argument",
			args: args{
				call: &FunctionCall{
					Name: "bytes4",
					Arguments: []*Argument{
						{
							Hex: &HexValue{
								String: utils.PointerOf("0x80ac58cd"),
							},
						},
					},
				},
				function: utils.PointerOf(packTestAbi.Methods["bytes4"]),
			},
			wantPacked: common.Hex2Bytes("f571323280ac58cd00000000000000000000000000000000000000000000000000000000"),
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			gotPacked, err := packFunction(tt.args.call.Arguments, tt.args.function)
			if (err != nil) != tt.wantErr {
				t.Errorf("packFunction() error = %v, wantErr %v", err, tt.wantErr)
				return
			}
			if !reflect.DeepEqual(gotPacked, tt.wantPacked) {
				t.Errorf("packFunction() = %v (= %s), want %v", gotPacked, common.Bytes2Hex(gotPacked), tt.wantPacked)
			}
		})
	}
}
