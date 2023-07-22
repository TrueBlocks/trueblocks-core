// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package identifiers

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
	"encoding/json"

	"github.com/alecthomas/participle/v2"
	"github.com/alecthomas/participle/v2/lexer"
)

// Define "tokens" for our lexer
var rangeLexer = lexer.MustSimple([]lexer.SimpleRule{
	{Name: `Date`, Pattern: `\d{4}-\d{2}-\d{2}(T[\d]{2}(:[\d]{2})?(:[\d]{2})?(UTC)?)?`},
	{Name: `Special`, Pattern: `[a-z_]+[0-9]*`},
	{Name: `Hash`, Pattern: `0x[a-f0-9]{64}`},
	{Name: `Hex`, Pattern: `0x[a-f0-9]+`},
	{Name: `Unsigned`, Pattern: `^[0-9]+`},
	{Name: `PointSeparator`, Pattern: `-`},
	{Name: `ModifierSeparator`, Pattern: `:`},
})

// A Point carries information about when a range starts or ends. It can be
// a block number, a date or special name (e.g. "london" is translated to
// block 12965000)
type Point struct {
	Number  uint   `parser:"@Hex|@Unsigned" json:"number,omitempty"`
	Hash    string `parser:"| @Hash" json:"hash,omitempty"`
	Date    string `parser:"| @Date" json:"date,omitempty"`
	Special string `parser:"| @Special" json:"special,omitempty"`
}

func (p Point) String() string {
	str, _ := json.MarshalIndent(p, "", "  ")
	return string(str)
}

// Modifier changes the meaning of the given range. For example, if step of
// 10 is provided, the range does not mean "each block from start to end"
// anymore, but instead "every 10th block from start to end". Similarly
// a period can be provided to get only blocks based on frequency (e.g. weekly)
type Modifier struct {
	Step   uint   `parser:"@Unsigned" json:"step,omitempty"`
	Period string `parser:"| @('hourly'|'daily'|'weekly'|'monthly'|'quarterly'|'annually'|'next'|'prev'|'all')" json:"period,omitempty"`
}

// Range is uses after having defined both Point and Modifier, we can construct
// our Range, which consist of a starting point, optionally followed by an ending
// point and optionally finished by a modifier. Separators for points and modifier
// are defined above as PointSeparator and ModifierSeparator
type Range struct {
	Points   []*Point  `parser:"@@('-'@@)?"`
	Modifier *Modifier `parser:"(':'@@)?"`
}

// Build parser
var parser = participle.MustBuild[Range](
	participle.Lexer(rangeLexer),
)

// Parse takes a string and tries to parse it into Range struct, which always have
// at least one Point (but no more than two: start and end) and may have
// Modifier.
func Parse(source string) (*Range, error) {
	blockRange, err := parser.ParseString("", source)

	return blockRange, err
}
