package parser

import (
	"errors"
	"fmt"
	"reflect"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/alecthomas/participle/v2/lexer"
	"github.com/ethereum/go-ethereum/accounts/abi"
	"github.com/ethereum/go-ethereum/common"
)

// ContractArgument represents input to the smart contract method call, e.g.
// `true` in `setSomething(true)`
type ContractArgument struct {
	Tokens  []lexer.Token // the token that was parsed for this argument
	String  *ArgString    `parser:"@String"`             // the value if it's a string
	Number  *ArgNumber    `parser:"| @Decimal"`          // the value if it's a number
	Boolean *ArgBool      `parser:"| @('true'|'false')"` // the value if it's a boolean
	Hex     *ArgHex       `parser:"| @Hex"`              // the value if it's a hex string
	EnsAddr *ArgEnsDomain `parser:"| @EnsDomain"`        // the value if it's an ENS domain
}

// Interface returns the value as interface{} (any)
func (a *ContractArgument) Interface() any {
	if a.EnsAddr != nil {
		return *a.EnsAddr
	}

	if a.String != nil {
		value := *a.String
		return string(value)
	}

	if a.Number != nil {
		return a.Number.Interface()
	}

	if a.Boolean != nil {
		return *a.Boolean
	}

	if a.Hex != nil {
		if a.Hex.Address != nil {
			return *a.Hex.Address
		}
		return *a.Hex.String
	}

	return nil
}

func (a *ContractArgument) AbiType(abiType *abi.Type) (any, error) {
	if abiType.T == abi.FixedBytesTy {
		// We only support fixed bytes as hashes
		if a.Hex == nil {
			return nil, wrongTypeError("hash", a.Tokens[0], a.Interface())
		}
		hex := *a.Hex.String
		if len(hex) == 0 {
			return nil, errors.New("no value for fixed-size bytes argument")
		}

		arrayInterface, err := abi.ReadFixedBytes(*abiType, base.Hex2Bytes(hex[2:]))
		if err != nil {
			return nil, err
		}
		return arrayInterface, nil
	}

	if abiType.T == abi.IntTy {
		if a.Number == nil {
			return nil, wrongTypeError(abiType.String(), a.Tokens[0], a.Interface())
		}
		// We have to convert int64 to a correct int type, otherwise go-ethereum will
		// return an error. It's not needed for uints, because they handle them differently.
		if a.Number.Big != nil {
			return a.Number.Interface(), nil
		}

		converted, err := a.Number.Convert(abiType)
		if err != nil {
			return nil, err
		}
		return converted, nil
	}

	if abiType.T == abi.AddressTy {
		if a.EnsAddr != nil {
			return a.EnsAddr, nil
		}

		// We need go-ethereum's Address type, not ours
		if a.Hex == nil {
			return nil, wrongTypeError(abiType.String(), a.Tokens[0], a.Interface())
		}
		address := a.Hex.Address
		if address == nil {
			return nil, errors.New("expected address, but got hash instead (check length)")
		}

		addressHex := common.HexToAddress(address.Hex())
		return addressHex, nil
	}

	// Below checks are for nice errors only
	if (abiType.T == abi.UintTy && a.Number == nil) ||
		(abiType.T == abi.StringTy && a.String == nil) ||
		(abiType.T == abi.BoolTy && a.Boolean == nil) {
		return nil, wrongTypeError(abiType.String(), a.Tokens[0], a.Interface())
	}

	return a.Interface(), nil
}

// wrongTypeError returns user-friendly errors
func wrongTypeError(expectedType string, token lexer.Token, value any) error {
	t := reflect.TypeOf(value)
	typeName := t.String()
	kind := t.Kind()
	// kinds between this range are all (u)int, called "integer" in Solidity
	if kind > 1 && kind < 12 {
		typeName = "integer"
	}
	return fmt.Errorf("expected %s, but got %s \"%s\"", expectedType, typeName, token)
}
