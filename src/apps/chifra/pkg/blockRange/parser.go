package blockRange

// This file provides a parser for block range strings.
//
// Supported formats are (including permutations):
// [blockNumber]
// [blockNumber]:[stepOrPeriod]
// [blockNumber]-[blockNumber]
// [blockNumber]-[blockNumber]:[stepOrPeriod]
// [special]
// [special]:[stepOrPeriod]
// [special]-[blockNumber]
// [special]-[blockNumber]:[stepOrPeriod]
// [special]-[special]
// [special]-[special]:[stepOrPeriod]
// [special]-[date]
// [special]-[date]:[stepOrPeriod]
// [date]
// [date]:[stepOrPeriod]
// [date]-[blockNumber]
// [date]-[blockNumber]:[stepOrPeriod]
// [date]-[special]
// [date]-[special]:[stepOrPeriod]
// [date]-[date]
// [date]-[date]:[stepOrPeriod]
//
// 10
// 10:100
// 10:weekly

// 10-1000
// 10-1000:100
// 10-1000:weekly

// 10-london
// 10-london:100
// 10-london:weekly

// 10-2021-10-03
// 10-2021-10-03T10
// 10-2021-10-03T10:30:59
// 10-2021-10-03T10:30:59:100

// 2021-10-03
// 2021-10-03T10:30:59
// 2021-10-03T10:30:59:100
// 2021-10-03T10:30:59-1000:100

import (
	"strconv"

	"github.com/alecthomas/participle/v2"
	"github.com/alecthomas/participle/v2/lexer"
)

// Define "tokens" for our lexer
var rangeLexer = lexer.MustSimple([]lexer.Rule{
	{`Date`, `\d{4}-\d{2}-\d{2}(T[\d]{2}(:[\d]{2})?(:[\d]{2})?(UTC)?)?`, nil},
	{`Special`, `[a-z_]+[0-9]*`, nil},
	{`Hex`, `0x[a-f0-9]+`, nil},
	{`Unsigned`, `^[0-9]+`, nil},
	{`PointSeparator`, `-`, nil},
	{`ModifierSeparator`, `:`, nil},
})

// chifra accepts block numbers with leading zeroes as decimals, which is different
// from Go (which treats them as octal) and this is why we have to add custom capturing
// for block numbers
type BlockNumber uint64

func (bn *BlockNumber) Capture(values []string) error {
	readValue := values[0]
	base := 10

	// Is it a hex?
	if len(readValue) > 2 && readValue[:2] == "0x" {
		base = 16
		readValue = readValue[2:]
	}

	// If base is set to 16, this will parse the hex value and return decimal.
	// If base is still 10, this will parse both decimals and decimals with
	// leading 0 (e.g. 024 meaning 24 and NOT octal 24, which would be decimal
	// 20)
	parsedValue, err := strconv.ParseUint(readValue, base, 64)
	*bn = BlockNumber(parsedValue)
	return err
}

// A Point carries information about when a range starts or ends. It can be
// a block number, a date or special name (e.g. "london" is translated to
// block 12965000)
type Point struct {
	Block   BlockNumber `parser:"@Hex|@Unsigned"`
	Date    string      `parser:"| @Date"`
	Special string      `parser:"| @Special"`
}

// Modifier changes the meaning of the given range. For example, if step of
// 10 is provided, the range does not mean "each block from start to end"
// anymore, but instead "every 10th block from start to end". Similarly
// a period can be provided to get only blocks based on frequency (e.g. weekly)
type Modifier struct {
	Step   uint   `parser:"@Unsigned"`
	Period string `parser:"| @('hourly'|'daily'|'weekly'|'monthly'|'quarterly'|'annually')"`
}

// Having defined both Point and Modifier, we can construct our Range, which
// consist of a starting point, optionally followed by an ending point and
// optionally finished by a modifier.
// Separators for points and modifier are defined above as PointSeparator and
// ModifierSeparator
type Range struct {
	Points   []*Point  `parser:"@@('-'@@)?"`
	Modifier *Modifier `parser:"(':'@@)?"`
}

// Build parser
var parser = participle.MustBuild(&Range{},
	participle.Lexer(rangeLexer),
)

// Takes a string and tries to parse it into Range struct, which always have
// at least one Point (but no more than two: start and end) and may have
// Modifier.
func Parse(source string) (*Range, error) {
	blockRange := &Range{}
	err := parser.ParseString("", source, blockRange)

	return blockRange, err
}
