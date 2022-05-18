// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blockRange

import (
	"encoding/json"
	"errors"
	"fmt"
	"regexp"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type BlockRangeValue int64

const (
	BlockNumber BlockRangeValue = iota
	BlockTimestamp
	BlockHash
	BlockDate
	BlockSpecial
	TransactionIndex
	TransactionHash
	Period
	Step
	NotDefined
)

type Identifier struct {
	StartType    BlockRangeValue `json:"startType,omitempty"`
	Start        Point           `json:"start,omitempty"`
	EndType      BlockRangeValue `json:"endType,omitempty"`
	End          Point           `json:"end,omitempty"`
	ModifierType BlockRangeValue `json:"modifierType,omitempty"`
	Modifier     Modifier        `json:"modifier,omitempty"`
	Orig         string          `json:"-"`
}

// Parses a string containing block range and returns a struct
// that always has Start, StartType, EndType, ModifierType fields
// and may as well have End and Modifier, if they are defined.
// *Type fields can be used to quickly check what is the type of
// the values.
//
// Consult ./parser.go for the supported format
func NewBlockRange(rangeStr string) (*Identifier, error) {
	parsed, err := Parse(rangeStr)
	newBlockRange := &Identifier{}
	if err != nil {
		return nil, handleParserErrors(err)
	}

	newBlockRange.Orig = rangeStr
	newBlockRange.StartType = getPointType(parsed.Points[0])
	newBlockRange.Start = *parsed.Points[0]

	if len(parsed.Points) == 1 {
		newBlockRange.EndType = NotDefined
	} else {
		newBlockRange.EndType = getPointType(parsed.Points[1])
		newBlockRange.End = *parsed.Points[1]
	}

	if parsed.Modifier == nil {
		newBlockRange.ModifierType = NotDefined
	} else {
		newBlockRange.ModifierType = getModifierType(parsed.Modifier)
		newBlockRange.Modifier = *parsed.Modifier
	}

	return newBlockRange, nil
}

func NewTxRange(rangeStr string) (*Identifier, error) {
	parsed, err := Parse(rangeStr)
	newBlockRange := &Identifier{}
	if err != nil {
		return nil, handleParserErrors(err)
	}

	newBlockRange.Orig = rangeStr
	newBlockRange.StartType = getPointType(parsed.Points[0])
	newBlockRange.Start = *parsed.Points[0]

	if len(parsed.Points) == 1 {
		newBlockRange.EndType = NotDefined
	} else {
		newBlockRange.EndType = getPointType(parsed.Points[1])
		newBlockRange.End = *parsed.Points[1]
	}

	if parsed.Modifier == nil {
		newBlockRange.ModifierType = NotDefined
	} else {
		newBlockRange.ModifierType = getModifierType(parsed.Modifier)
		newBlockRange.Modifier = *parsed.Modifier
	}

	return newBlockRange, nil
}

func (brv BlockRangeValue) String() string {
	return []string{
		"BlockNumber",
		"BlockTimestamp",
		"BlockHash",
		"BlockDate",
		"BlockSpecial",
		"TransactionIndex",
		"TransactionHash",
		"Period",
		"Step",
		"NotDefined",
	}[brv]
}

func (br Identifier) ToJSON() string {
	str, err := json.Marshal(br)
	if err != nil {
		return ""
	}
	return string(str)
}

func (br *Identifier) UnmarshalJSON(data []byte) error {
	str, err := strconv.Unquote(string(data))
	if err != nil {
		return err
	}

	newBlock, err := NewBlockRange(str)
	if err != nil {
		return err
	}

	*br = *newBlock

	return nil
}

func (br *Identifier) String() string {
	return br.ToJSON()
}

func getPointType(p *Point) BlockRangeValue {
	if p == nil {
		return NotDefined
	}

	if p.Date != "" {
		return BlockDate
	}

	if p.Special != "" {
		return BlockSpecial
	}

	if p.Hash != "" {
		return BlockHash
	}

	if p.Number >= utils.EarliestTs {
		return BlockTimestamp
	}

	return BlockNumber
}

func getModifierType(m *Modifier) BlockRangeValue {
	if m == nil {
		return Step
	}

	if m.Period != "" {
		return Period
	}

	return Step
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
