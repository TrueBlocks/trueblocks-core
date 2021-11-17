/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
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

// A Point carries information about when a range starts or ends. It can be
// a block number, a date or special name (e.g. "london" is translated to
// block 12965000)
type Point struct {
	Block   uint   `parser:"@Hex|@Unsigned"`
	Date    string `parser:"| @Date"`
	Special string `parser:"| @Special"`
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
