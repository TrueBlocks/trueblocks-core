package parser

import (
	"errors"
	"fmt"
	"math/big"
	"reflect"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/alecthomas/participle/v2"
	"github.com/alecthomas/participle/v2/lexer"
	"github.com/ethereum/go-ethereum/accounts/abi"
	"github.com/ethereum/go-ethereum/common"
)

var errInvalidSelector = errors.New("expected valid four byte selector")

// Input syntax:
// 1. Everything encoded (4-byte + parameters):
// chifra state --call 0xcdba2fd40000000000000000000000000000000000000000000000000000000000007a69
// 2. encoded 4-byte only
// chifra state --call 0xdeadbeef("hello", 110)
// 3. signature + params
// chifra state --call readMessage("hello", 110)

// Define "tokens" for our lexer
var contractCallLexer = lexer.MustSimple([]lexer.SimpleRule{
	{Name: `Hex`, Pattern: `0x[[:xdigit:]]+`},

	// https://docs.soliditylang.org/en/v0.8.17/grammar.html#a4.SolidityLexer.Identifier
	{Name: `SolidityIdent`, Pattern: `[a-zA-Z$_][a-zA-Z0-9$_]*`},
	// Values:
	{Name: `String`, Pattern: `"(?:\\.|[^"])*"`},
	{Name: `Decimal`, Pattern: `[-+]?\d+`},

	{Name: `whitespace`, Pattern: `\s+`},
	{Name: `Punctation`, Pattern: `[(),]`},
})

// ContractCall is a call to a smart contract in any of the 3 supported forms
// (see Input syntax above)
type ContractCall struct {
	// Four byte selector, e.g.
	// 0xcdba2fd4(105)
	SelectorCall *SelectorContractCall `parser:"@@"`

	// Everything encoded, e.g.
	// 0xcdba2fd40000000000000000000000000000000000000000000000000000000000007a69
	Encoded string `parser:" | @Hex"`

	// Function name, e.g.
	// someName(105)
	FunctionNameCall *FunctionContractCall `parser:" | @@"`
}

type SelectorContractCall struct {
	Selector  Selector                `parser:"@Hex '('"`
	Arguments []*ContractCallArgument `parser:" (@@ (',' @@)*)? ')'"`
}

type FunctionContractCall struct {
	Name      string                  `parser:"@SolidityIdent '('"`
	Arguments []*ContractCallArgument `parser:" (@@ (',' @@)*)? ')'"`
}

// ContractCallArgument represents input to the smart contract method call, e.g.
// `true` in `setSomething(true)`
type ContractCallArgument struct {
	Tokens []lexer.Token

	String  *string             `parser:"@String"`
	Number  *ContractCallNumber `parser:"| @Decimal"`
	Boolean *Boolean            `parser:"| @('true'|'false')"`
	Hex     *ContractCallHex    `parser:"| @Hex"`
}

// Interface returns the value as interface{} (any)
func (a *ContractCallArgument) Interface() any {
	if a.String != nil {
		return *a.String
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

func (a *ContractCallArgument) AbiType(abiType *abi.Type) (any, error) {
	if abiType.T == abi.FixedBytesTy {
		// We only support fixed bytes as hashes
		if a.Hex == nil {
			return nil, newWrongTypeError("hash", a.Tokens[0], a.Interface())
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
			return nil, newWrongTypeError(abiType.String(), a.Tokens[0], a.Interface())
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
		// We need go-ethereum's Address type, not ours
		if a.Hex == nil {
			return nil, newWrongTypeError(abiType.String(), a.Tokens[0], a.Interface())
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
		return nil, newWrongTypeError(abiType.String(), a.Tokens[0], a.Interface())
	}

	return a.Interface(), nil
}

// Boolean is a type alias to capture bool values correctly
type Boolean bool

func (b *Boolean) Capture(values []string) error {
	*b = values[0] == "true"
	return nil
}

// ContractCallHex represents anything that starts with 0x. If the value is a valid
// address, then it's capture into `base.Address` type, `string` otherwise.
type ContractCallHex struct {
	Address *base.Address
	String  *string
}

func (h *ContractCallHex) Capture(values []string) error {
	hexLiteral := values[0]

	if valid, _ := base.IsValidHex("", hexLiteral, 20); !valid {
		h.String = &hexLiteral
		return nil
	}

	address := base.HexToAddress(hexLiteral)
	h.Address = &address
	return nil
}

type ContractCallNumber struct {
	Int  *int64
	Uint *uint64
	Big  *big.Int
}

func (n *ContractCallNumber) Capture(values []string) error {
	literal := values[0]

	// Atoi parses into `int` type, which is used by go-ethereum
	// to construct solidity int types.
	asInt, err := strconv.ParseInt(literal, 10, 64)
	if err == nil {
		n.Int = &asInt
		return nil
	}

	asUint, err := strconv.ParseUint(literal, 10, 64)
	if err == nil {
		n.Uint = &asUint
		return nil
	}

	// If we are here, the number is bigger than int64
	asBig := big.NewInt(0)
	_, ok := asBig.SetString(literal, 10)
	if ok {
		n.Big = asBig
		return nil
	}

	return fmt.Errorf("cannot parse %s as number", literal)
}

// Interface returns Number value as any
func (n *ContractCallNumber) Interface() any {
	if n.Int != nil {
		return *n.Int
	}
	if n.Uint != nil {
		return *n.Uint
	}
	return n.Big
}

func (n *ContractCallNumber) Convert(abiType *abi.Type) (any, error) {
	if abiType.Size > 64 {
		return n.Big, nil
	}

	if abiType.T == abi.UintTy {
		switch abiType.Size {
		case 8:
			return uint8(*n.Uint), nil
		case 16:
			return uint16(*n.Uint), nil
		case 32:
			return uint32(*n.Uint), nil
		case 64:
			return uint64(*n.Uint), nil
		}
	} else if abiType.T == abi.IntTy {
		switch abiType.Size {
		case 8:
			return int8(*n.Int), nil
		case 16:
			return int16(*n.Int), nil
		case 32:
			return int32(*n.Int), nil
		case 64:
			return int64(*n.Int), nil
		}
	}

	return nil, fmt.Errorf("cannot convert %v to number", n.Interface())
}

// Selector captures four byte function selector (e.g. 0xcdba2fd4)
// It's capture method makes sure that the hex value is a valid selector.
type Selector struct {
	Value string
}

func (s *Selector) Capture(values []string) error {
	literal := values[0]
	if valid, _ := base.IsValidHex("", literal, 4); !valid {
		return errInvalidSelector
	}

	s.Value = literal
	return nil
}

func newWrongTypeError(expectedType string, token lexer.Token, value any) error {
	t := reflect.TypeOf(value)
	typeName := t.String()
	kind := t.Kind()
	// kinds between this range are all (u)int, called "integer" in Solidity
	if kind > 1 && kind < 12 {
		typeName = "integer"
	}
	return fmt.Errorf("expected %s, but got %s \"%s\"", expectedType, typeName, token)
}

// Build parser
var parser = participle.MustBuild[ContractCall](
	participle.Lexer(contractCallLexer),
	participle.UseLookahead(2),
	participle.Unquote("String"),
)

// ParseContractCall turns smart contract method call string and parses it into
// a nice structures, from which we can easily extract the data to
// make the call.
func ParseContractCall(source string) (*ContractCall, error) {
	callSpec, err := parser.ParseString("", source)

	return callSpec, err
}
