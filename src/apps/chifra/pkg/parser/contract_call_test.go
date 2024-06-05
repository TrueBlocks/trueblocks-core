package parser

import (
	"errors"
	"reflect"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/accounts/abi"
	"github.com/ethereum/go-ethereum/common"
)

func TestParse_Encoding(t *testing.T) {
	// Short
	if p, err := ParseCall(`0xcdba2fd40000000000000000000000000000000000000000000000000000000000007a69`); err != nil {
		t.Fatal(err)
	} else {
		if s := p.Encoded; s != `0xcdba2fd40000000000000000000000000000000000000000000000000000000000007a69` {
			t.Fatal("wrong encoded value", s)
		}
	}

	// Long one
	long := "0x7087e4bd000000000000000000000000f503017d7baf7fbc0fff7492b751025c6a78179b000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000076d61696e6e657400000000000000000000000000000000000000000000000000"
	if p, err := ParseCall(long); err != nil {
		t.Fatal(err)
	} else {
		if s := p.Encoded; s != long {
			t.Fatal("wrong encoded value", s)
		}
	}
}

func TestParse_Selector(t *testing.T) {
	// Invalid selector
	if _, err := ParseCall(`0xcdba()`); !strings.Contains(err.Error(), errInvalidSelector.Error()) {
		t.Fatal("expected errInvalidSelector, got:", err)
	}

	// No arguments
	if parsed, err := ParseCall(`0xcdba2fd4()`); err != nil {
		t.Fatal(err)
	} else {
		if value := parsed.SelectorCall.Selector.Value; value != `0xcdba2fd4` {
			t.Fatal("wrong selector", value)
		}
		if argsLen := len(parsed.SelectorCall.Arguments); argsLen != 0 {
			t.Fatal("wrong number of arguments", argsLen)
		}
	}

	if parsed, err := ParseCall(`0x7087e4bd(trueblocks.eth,"mainnet")`); err != nil {
		t.Fatal(err)
	} else {
		if value := parsed.SelectorCall.Selector.Value; value != `0x7087e4bd` {
			t.Fatal("wrong selector", value)
		}
		if argsLen := len(parsed.SelectorCall.Arguments); argsLen != 2 {
			t.Fatal("wrong number of arguments", argsLen)
		}
	}

	// Arguments
	if parsed, err := ParseCall(`0xcdba2fd4(1, true, false, 0xbeef, "string")`); err != nil {
		t.Fatal(err)
	} else {
		if value := parsed.SelectorCall.Selector.Value; value != `0xcdba2fd4` {
			t.Fatal("wrong selector", value)
		}
		if argValue := *parsed.SelectorCall.Arguments[0].Number.Int; argValue != 1 {
			t.Fatal("wrong #1 input value", argValue)
		}
		if argValue := *parsed.SelectorCall.Arguments[1].Boolean; argValue != true {
			t.Fatal("wrong #2 input value", argValue)
		}
		if argValue := *parsed.SelectorCall.Arguments[2].Boolean; argValue != false {
			t.Fatal("wrong #3 input value", argValue)
		}
		if argValue := *parsed.SelectorCall.Arguments[3].Hex.String; argValue != "0xbeef" {
			t.Fatal("wrong #4 input value", argValue)
		}
		if argValue := *parsed.SelectorCall.Arguments[4].String; argValue != "string" {
			t.Fatal("wrong #5 input value", argValue)
		}
	}

	// Hex parsing
	if parsed, err := ParseCall(`0xcdba2fd4(0xdeadbeef, 0x6982508145454ce325ddbe47a25d4ec3d23119a1)`); err != nil {
		t.Fatal(err)
	} else {
		if argsLen := len(parsed.SelectorCall.Arguments); argsLen != 2 {
			t.Fatal("wrong inputs length:", argsLen)
		}

		if *parsed.SelectorCall.Arguments[0].Hex.String != "0xdeadbeef" {
			t.Fatal("should parse into Hex.String")
		}
		if parsed.SelectorCall.Arguments[1].Hex.Address.Hex() != "0x6982508145454ce325ddbe47a25d4ec3d23119a1" {
			t.Fatal("should parse into Hex.Address")
		}
	}
}

func TestParse_Function(t *testing.T) {
	// Invalid selector
	if _, err := ParseCall(`111()`); err == nil {
		t.Fatal("expected parsing error")
	}

	// No arguments
	if parsed, err := ParseCall(`transfer()`); err != nil {
		t.Fatal(err)
	} else {
		if value := parsed.FunctionNameCall.Name; value != `transfer` {
			t.Fatal("wrong function name", value)
		}
		if argsLen := len(parsed.FunctionNameCall.Arguments); argsLen != 0 {
			t.Fatal("wrong number of arguments", argsLen)
		}
	}

	// Correct Solidity identifiers
	if parsed, err := ParseCall(`$dollar$_underscoreCamelCase125__()`); err != nil {
		t.Fatal(err)
	} else {
		if value := parsed.FunctionNameCall.Name; value != `$dollar$_underscoreCamelCase125__` {
			t.Fatal("wrong function name", value)
		}
	}

	// Arguments
	if parsed, err := ParseCall(`something(1, true, false, 0xbeef, "string")`); err != nil {
		t.Fatal(err)
	} else {
		if value := parsed.FunctionNameCall.Name; value != `something` {
			t.Fatal("wrong selector", value)
		}
		if argValue := *parsed.FunctionNameCall.Arguments[0].Number.Int; argValue != 1 {
			t.Fatal("wrong #1 input value", argValue)
		}
		if argValue := *parsed.FunctionNameCall.Arguments[1].Boolean; argValue != true {
			t.Fatal("wrong #2 input value", argValue)
		}
		if argValue := *parsed.FunctionNameCall.Arguments[2].Boolean; argValue != false {
			t.Fatal("wrong #3 input value", argValue)
		}
		if argValue := *parsed.FunctionNameCall.Arguments[3].Hex.String; argValue != "0xbeef" {
			t.Fatal("wrong #4 input value", argValue)
		}
		if argValue := *parsed.FunctionNameCall.Arguments[4].String; argValue != "string" {
			t.Fatal("wrong #5 input value", argValue)
		}
	}

	// Hex parsing
	if parsed, err := ParseCall(`somethingElse(0xdeadbeef, 0x6982508145454ce325ddbe47a25d4ec3d23119a1)`); err != nil {
		t.Fatal(err)
	} else {
		if argsLen := len(parsed.FunctionNameCall.Arguments); argsLen != 2 {
			t.Fatal("wrong inputs length:", argsLen)
		}

		if *parsed.FunctionNameCall.Arguments[0].Hex.String != "0xdeadbeef" {
			t.Fatal("should parse into Hex.String")
		}
		if parsed.FunctionNameCall.Arguments[1].Hex.Address.Hex() != "0x6982508145454ce325ddbe47a25d4ec3d23119a1" {
			t.Fatal("should parse into Hex.Address")
		}
	}
}

func TestParse_Numbers(t *testing.T) {
	if parsed, err := ParseCall(`doSomething(1, -2, 115792089237316195423570985008687907853269984665640564039457584007913129639935)`); err != nil {
		t.Fatal(err)
	} else {
		if argsLen := len(parsed.FunctionNameCall.Arguments); argsLen != 3 {
			t.Fatal("wrong inputs length:", argsLen)
		}

		if value := *parsed.FunctionNameCall.Arguments[0].Number.Int; value != 1 {
			t.Fatal("wrong uint value:", value)
		}
		if value := *parsed.FunctionNameCall.Arguments[1].Number.Int; value != -2 {
			t.Fatal("wrong int value:", value)
		}
		if value := parsed.FunctionNameCall.Arguments[2].Number.Big.String(); value != "115792089237316195423570985008687907853269984665640564039457584007913129639935" {
			t.Fatal("wrong big value:", value)
		}
	}
}

// This comes from go-ethereum's abi_test.go, we will use it to
// test type conversion (our types -> go-ethereum)
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

func TestArgument_AbiType(t *testing.T) {
	testAbi, err := abi.JSON(strings.NewReader(packTestAbiSource))
	if err != nil {
		panic(err)
	}
	int256 := base.NewWei(0)
	_, ok := int256.SetString("-57896044618658097711785492504343953926634992332820282019728792003956564819968", 0)
	if !ok {
		t.Fatal("cannot set int256 value")
	}

	type fields struct {
		String  *ArgString
		Number  *ArgNumber
		Boolean *ArgBool
		Hex     *ArgHex
	}
	type args struct {
		abiType *abi.Type
	}
	tests := []struct {
		name    string
		fields  fields
		args    args
		want    any
		wantErr bool
	}{
		{
			name: "bytes4 argument",
			fields: fields{
				Hex: &ArgHex{
					String: utils.PointerOf("0x80ac58cd"),
				},
			},
			args: args{
				abiType: &testAbi.Methods["bytes4"].Inputs[0].Type,
			},
			want: [4]byte{128, 172, 88, 205},
		},
		{
			name: "int8 argument",
			fields: fields{
				Number: &ArgNumber{
					Int: utils.PointerOf(int64(-128)),
				},
			},
			args: args{
				abiType: &testAbi.Methods["int8"].Inputs[0].Type,
			},
			want: int8(-128),
		},
		{
			name: "int64 argument",
			fields: fields{
				Number: &ArgNumber{
					Int: utils.PointerOf(int64(-9223372036854775808)),
				},
			},
			args: args{
				abiType: &testAbi.Methods["int64"].Inputs[0].Type,
			},
			want: int64(-9223372036854775808),
		},
		{
			name: "int256 argument",
			fields: fields{
				Number: &ArgNumber{
					Big: int256,
				},
			},
			args: args{
				abiType: &testAbi.Methods["int256"].Inputs[0].Type,
			},
			want: int256,
		},
		{
			name: "address argument",
			fields: fields{
				Hex: &ArgHex{
					Address: utils.PointerOf(base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b")),
				},
			},
			args: args{
				abiType: &testAbi.Methods["address"].Inputs[0].Type,
			},
			want: common.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"),
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			a := &ContractArgument{
				String:  tt.fields.String,
				Number:  tt.fields.Number,
				Boolean: tt.fields.Boolean,
				Hex:     tt.fields.Hex,
			}
			got, err := a.AbiType(tt.args.abiType)
			if (err != nil) != tt.wantErr {
				t.Errorf("Argument.ToAbiType() error = %v, wantErr %v", err, tt.wantErr)
				return
			}
			if !reflect.DeepEqual(got, tt.want) {
				t.Errorf("Argument.ToAbiType() = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestArgument_AbiType_Errors(t *testing.T) {
	testAbi, err := abi.JSON(strings.NewReader(packTestAbiSource))
	if err != nil {
		panic(err)
	}

	// the second argument is string instead of address
	parsed, err := ParseCall(`someFunc(0x6982508145454ce325ddbe47a25d4ec3d23119a1, "0x6982508145454ce325ddbe47a25d4ec3d23119a1")`)
	if err != nil {
		t.Fatal(err)
	}
	if _, err := parsed.FunctionNameCall.Arguments[0].AbiType(&testAbi.Methods["address"].Inputs[0].Type); err != nil {
		t.Fatal(err)
	}
	_, err = parsed.FunctionNameCall.Arguments[1].AbiType(&testAbi.Methods["address"].Inputs[0].Type)
	expected := errors.New(`expected address, but got string "0x6982508145454ce325ddbe47a25d4ec3d23119a1"`)
	if err.Error() != expected.Error() {
		t.Fatal("got wrong error:", err, "expected:", expected)
	}

	parsed, err = ParseCall(`someBool(111)`)
	if err != nil {
		t.Fatal(err)
	}
	_, err = parsed.FunctionNameCall.Arguments[0].AbiType(&testAbi.Methods["bool"].Inputs[0].Type)
	expected = errors.New(`expected bool, but got integer "111"`)
	if err.Error() != expected.Error() {
		t.Fatal("got wrong error:", err, "expected:", expected)
	}

	parsed, err = ParseCall(`someBytes32("hello")`)
	if err != nil {
		t.Fatal(err)
	}
	_, err = parsed.FunctionNameCall.Arguments[0].AbiType(&testAbi.Methods["bytes32"].Inputs[0].Type)
	expected = errors.New(`expected hash, but got string "hello"`)
	if err.Error() != expected.Error() {
		t.Fatal("got wrong error:", err, "expected:", expected)
	}
}
