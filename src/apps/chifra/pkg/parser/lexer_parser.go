package parser

import (
	"github.com/alecthomas/participle/v2"
	"github.com/alecthomas/participle/v2/lexer"
)

// Input syntax:
//
// - Input data:                chifra state --call '0xcdba2fd40000000000000000000000000000000000000000000000000000000000007a69'
// - Four-byte plus params:     chifra state --call '0xdeadbeef("hello", 110)'
// - Function name plus params: chifra state --call 'readMessage("hello", 110)'

// This defines the tokens our lexer will match on. For the SolidityIdent, see
// https://docs.soliditylang.org/en/v0.8.17/grammar.html#a4.SolidityLexer.Identifier
var ourLexer = lexer.MustSimple([]lexer.SimpleRule{
	// matched lexer tokens (order matters)
	{Name: `EnsDomain`, Pattern: `([a-zA-Z0-9]+)(\.[a-zA-Z0-9]+)*\.eth`}, // TODO: this token does not handle dashes
	{Name: `Hex`, Pattern: `0x[[:xdigit:]]+`},
	{Name: `String`, Pattern: `"(?:\\.|[^"])*"`},
	{Name: `Decimal`, Pattern: `[-+]?\d+`},
	{Name: `SolidityIdent`, Pattern: `[a-zA-Z$_][a-zA-Z0-9$_]*`},
	{Name: `whitespace`, Pattern: `\s+`},
	{Name: `Punctation`, Pattern: `[(),]`},
})

// This builds our parser from the lexer and some options
var ourParser = participle.MustBuild[ContractCall](
	participle.Lexer(ourLexer),
	participle.UseLookahead(2),
	participle.Unquote("String"),
)
