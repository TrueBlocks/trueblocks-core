package types

import (
	"math/big"
	"reflect"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/ethereum/go-ethereum/accounts/abi"
)

func TestFunctionToAbiMethod(t *testing.T) {
	input := &Function{
		Name:         "deploy",
		FunctionType: "function",
		Inputs: []Parameter{
			{
				Name:          "where",
				ParameterType: "address",
			},
			{
				Name:          "options",
				ParameterType: "tuple",
				InternalType:  "struct Options",
				Components: []Parameter{
					{Name: "force", ParameterType: "bool"},
					{Name: "mode", ParameterType: "uint8"},
				},
			},
		},
	}
	result, err := input.AbiMethodFromFunction()
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

func TestPack(t *testing.T) {
	const abiSource = `
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
	abi, err := abi.JSON(strings.NewReader(abiSource))
	if err != nil {
		t.Fatal(err)
	}
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

	functions := make(map[string]Function)
	for _, abiMethod := range abi.Methods {
		functions[abiMethod.Name] = *FunctionFromAbiMethod(&abiMethod)
	}

	a := base.HexToAddress("0xE2e87901a3E1CB22b9853AE0827DE34E6Fae0575")
	addr := a.Common()

	type args struct {
		callArguments []any
		function      Function
	}
	tests := []struct {
		name    string
		args    args
		want    []byte
		wantErr bool
	}{
		{
			name: "string argument",
			args: args{
				callArguments: []any{
					"hello",
				},
				function: functions["string"],
			},
			want: base.Hex2Bytes("3fcf74c60000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000000568656c6c6f000000000000000000000000000000000000000000000000000000"),
		},
		{
			name: "boolean argument",
			args: args{
				callArguments: []any{
					true,
				},
				function: functions["bool"],
			},
			want: base.Hex2Bytes("17192efc0000000000000000000000000000000000000000000000000000000000000001"),
		},
		{
			name: "address argument",
			args: args{
				callArguments: []any{addr},
				function:      functions["address"],
			},
			want: base.Hex2Bytes("0706105e000000000000000000000000e2e87901a3e1cb22b9853ae0827de34e6fae0575"),
		},
		{
			name: "uint64 argument",
			args: args{
				callArguments: []any{
					uint64(18446744073709551615),
				},
				function: functions["uint64"],
			},
			want: base.Hex2Bytes("8ab008da000000000000000000000000000000000000000000000000ffffffffffffffff"),
		},
		{
			name: "uint256 argument",
			args: args{
				callArguments: []any{
					uint256,
				},
				function: functions["uint256"],
			},
			want: base.Hex2Bytes("6449cd53ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"),
		},
		{
			name: "int8 argument",
			args: args{
				callArguments: []any{
					int8(-128),
				},
				function: functions["int8"],
			},
			want: base.Hex2Bytes("22aca450ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff80"),
		},
		{
			name: "int64 argument",
			args: args{
				callArguments: []any{
					int64(-9223372036854775808),
				},
				function: functions["int64"],
			},
			want: base.Hex2Bytes("4949bf43ffffffffffffffffffffffffffffffffffffffffffffffff8000000000000000"),
		},
		{
			name: "int256 argument",
			args: args{
				callArguments: []any{
					int256,
				},
				function: functions["int256"],
			},
			want: base.Hex2Bytes("4280896d8000000000000000000000000000000000000000000000000000000000000000"),
		},
		{
			name: "fixed-bytes argument",
			args: args{
				callArguments: []any{
					[4]byte{128, 172, 88, 205},
				},
				function: functions["bytes4"],
			},
			want: base.Hex2Bytes("f571323280ac58cd00000000000000000000000000000000000000000000000000000000"),
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			gotPacked, err := tt.args.function.Pack(tt.args.callArguments)
			if (err != nil) != tt.wantErr {
				t.Errorf("function.Pack() error = %v, wantErr %v", err, tt.wantErr)
				return
			}
			if !reflect.DeepEqual(gotPacked, tt.want) {
				t.Errorf("function.Pack() = %v (= %s), want %v", gotPacked, base.Bytes2Hex(gotPacked), tt.want)
			}
		})
	}
}
