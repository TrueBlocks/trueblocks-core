package parser

import (
	"errors"
	"fmt"
	"reflect"

	"github.com/alecthomas/participle/v2"
	"github.com/alecthomas/participle/v2/lexer"
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
	// https://docs.soliditylang.org/en/v0.8.17/grammar.html#a4.SolidityLexer.Identifier
	{Name: `SolidityIdent`, Pattern: `[a-zA-Z$_][a-zA-Z0-9$_]*`},

	// Values types
	{Name: `Hex`, Pattern: `0x[[:xdigit:]]+`},
	{Name: `String`, Pattern: `"(?:\\.|[^"])*"`},
	{Name: `Decimal`, Pattern: `[-+]?\d+`},

	// Whitespace and punctuation
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
	Selector  Selector            `parser:"@Hex '('"`
	Arguments []*ContractArgument `parser:" (@@ (',' @@)*)? ')'"`
}

type FunctionContractCall struct {
	Name      string              `parser:"@SolidityIdent '('"`
	Arguments []*ContractArgument `parser:" (@@ (',' @@)*)? ')'"`
}

// Build parser
var parser = participle.MustBuild[ContractCall](
	participle.Lexer(contractCallLexer),
	participle.UseLookahead(2),
	participle.Unquote("String"),
)

// ParseCall turns smart contract method call string and parses it into
// a nice structures, from which we can easily extract the data to
// make the call.
func ParseCall(source string) (*ContractCall, error) {
	return parser.ParseString("", source)
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
