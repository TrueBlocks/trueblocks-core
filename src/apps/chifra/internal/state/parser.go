package statePkg

import (
	"errors"
	"fmt"
	"math/big"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/alecthomas/participle/v2"
	"github.com/alecthomas/participle/v2/lexer"
)

var errInvalidSelector = errors.New("expected valid four byte selector")

// Input syntax:
// 1. Everything encoded (4-byte + parameters):
// chifra state --call 0xcdba2fd40000000000000000000000000000000000000000000000000000000000007a69
// 2. encoded 4-byte only
// chifra state --call 0xdeadbeef("yolo", 110)
// 3. signature + params
// chifra state --call readMessage("yolo", 110)

// Define "tokens" for our lexer
var callLexer = lexer.MustSimple([]lexer.SimpleRule{
	{Name: `Encoded`, Pattern: `0x[[:xdigit:]]{72}`},
	{Name: `Hex`, Pattern: `0x[[:xdigit:]]+`},

	// https://docs.soliditylang.org/en/v0.8.17/grammar.html#a4.SolidityLexer.Identifier
	{Name: `SolidityIdent`, Pattern: `[a-zA-Z$_][a-zA-Z0-9$_]*`},
	// Values:
	{Name: `String`, Pattern: `"(?:\\.|[^"])*"`},
	{Name: `Decimal`, Pattern: `[-+]?\d+`},

	{Name: `whitespace`, Pattern: `\s+`},
	{Name: `Punctation`, Pattern: `[(),]`},
})

// The call is any of the 3 supported forms (see Input syntax above)
type Call struct {
	// Everything encoded
	Encoded string `parser:"@Encoded"`
	// Four byte selector
	SelectorCall *SelectorCall `parser:" | @@"`
	// Function name
	FunctionNameCall *FunctionCall `parser:" | @@"`
}

type SelectorCall struct {
	Selector  Selector    `parser:"@Hex '('"`
	Arguments []*Argument `parser:" (@@ (',' @@)*)? ')'"`
}

type FunctionCall struct {
	Name      string      `parser:"@SolidityIdent '('"`
	Arguments []*Argument `parser:" (@@ (',' @@)*)? ')'"`
}

// Argument represents input to the smart contract method call, e.g.
// `true` in `setSomething(true)`
type Argument struct {
	String  *string   `parser:"@String"`
	Number  *Number   `parser:"| @Decimal"`
	Boolean *Boolean  `parser:"| @('true'|'false')"`
	Hex     *HexValue `parser:"| @Hex"`
}

// Interface returns the value as interface{} (any)
func (a *Argument) Interface() any {
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

// Type alias to capture bool values correctly
type Boolean bool

func (b *Boolean) Capture(values []string) error {
	*b = values[0] == "true"
	return nil
}

// HexValue represents anything that starts with 0x. If the value is a valid
// address, then it's capture into `base.Address` type, `string` otherwise.
type HexValue struct {
	Address *base.Address
	String  *string
}

func (h *HexValue) Capture(values []string) error {
	hexLiteral := values[0]

	if valid, _ := validate.IsValidHex("", hexLiteral, 20); !valid {
		h.String = &hexLiteral
		return nil
	}

	address := base.HexToAddress(hexLiteral)
	h.Address = &address
	return nil
}

type Number struct {
	Int  *int64
	Uint *uint64
	Big  *big.Int
}

func (n *Number) Capture(values []string) error {
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
func (n *Number) Interface() any {
	if n.Int != nil {
		return *n.Int
	}
	if n.Uint != nil {
		return *n.Uint
	}
	return n.Big
}

// Selector captures four byte function selector (e.g. 0xcdba2fd4)
// It's capture method makes sure that the hex value is a valid selector.
type Selector struct {
	Value string
}

func (s *Selector) Capture(values []string) error {
	literal := values[0]
	if valid, _ := validate.IsValidHex("", literal, 4); !valid {
		return errInvalidSelector
	}

	s.Value = literal
	return nil
}

// Build parser
var parser = participle.MustBuild[Call](
	participle.Lexer(callLexer),
	participle.UseLookahead(2),
	participle.Unquote("String"),
)

// Parse turns smart contract method call string and parses it into
// a nice structures, from which we can easily extract the data to
// make the call.
func Parse(source string) (*Call, error) {
	callSpec, err := parser.ParseString("", source)

	return callSpec, err
}
