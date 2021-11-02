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

import (
	"errors"
	"fmt"
	"regexp"
)

// Parses a string containing block range and returns a struct
// that always has Start, StartType, EndType, ModifierType fields
// and may as well have End and Modifier, if they are defined.
// *Type fields can be used to quickly check what is the type of
// the values.
//
// Consult ./parser.go for the supported format
func New(rangeStr string) (*BlockRange, error) {
	parsed, err := Parse(rangeStr)
	newBlockRange := &BlockRange{}

	if err != nil {
		return nil, handleParserErrors(err)
	}

	newBlockRange.StartType = getPointType(parsed.Points[0])
	newBlockRange.Start = *parsed.Points[0]

	if len(parsed.Points) == 1 {
		newBlockRange.EndType = BlockRangeNotDefined
	} else {
		newBlockRange.EndType = getPointType(parsed.Points[1])
		newBlockRange.End = *parsed.Points[1]
	}

	if parsed.Modifier == nil {
		newBlockRange.ModifierType = BlockRangeNotDefined
	} else {
		newBlockRange.ModifierType = getModifierType(parsed.Modifier)
		newBlockRange.Modifier = *parsed.Modifier
	}

	return newBlockRange, nil
}

type BlockRangeValue int64

const (
	BlockRangeBlockNumber BlockRangeValue = iota
	BlockRangeDate
	BlockRangeSpecial
	BlockRangePeriod
	BlockRangeStep
	BlockRangeNotDefined
)

type BlockRange struct {
	StartType    BlockRangeValue
	Start        Point
	EndType      BlockRangeValue
	End          Point
	ModifierType BlockRangeValue
	Modifier     Modifier
}

func getPointType(p *Point) BlockRangeValue {
	if p == nil {
		return BlockRangeNotDefined
	}

	if p.Date != "" {
		return BlockRangeDate
	}

	if p.Special != "" {
		return BlockRangeSpecial
	}

	return BlockRangeBlockNumber
}

func getModifierType(m *Modifier) BlockRangeValue {
	if m == nil {
		return BlockRangeStep
	}

	if m.Period != "" {
		return BlockRangePeriod
	}

	return BlockRangeStep
}

type WrongModifierError struct {
	Token string
}

func (e *WrongModifierError) Error() string {
	return fmt.Sprintf("wrong modifier: %s", e.Token)
}

func handleParserErrors(parseError error) error {
	modifierMatch, err := regexp.MatchString("expected Modifier", parseError.Error())

	if err != nil || !modifierMatch {
		return parseError
	}

	matches := regexp.MustCompile(`unexpected token "(.+)"`).FindStringSubmatch(parseError.Error())

	if len(matches) < 2 {
		return errors.New("no matches")
	}

	return &WrongModifierError{
		Token: matches[1],
	}
}
