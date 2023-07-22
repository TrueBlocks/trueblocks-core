// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package identifiers

import (
	"encoding/json"
	"errors"
	"fmt"
	"regexp"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type IdentifierType int64

const (
	NotDefined IdentifierType = iota
	BlockNumber
	BlockTimestamp
	BlockHash
	BlockDate
	BlockSpecial
	TransactionIndex
	TransactionHash
	Period
	Step
)

type Identifier struct {
	StartType    IdentifierType `json:"startType,omitempty"`
	Start        Point          `json:"start,omitempty"`
	EndType      IdentifierType `json:"endType,omitempty"`
	End          Point          `json:"end,omitempty"`
	ModifierType IdentifierType `json:"modifierType,omitempty"`
	Modifier     Modifier       `json:"modifier,omitempty"`
	Orig         string         `json:"-"`
}

// NewBlockRange parses a string containing block range and returns a struct
// that always has Start, StartType, EndType, ModifierType fields
// and may as well have End and Modifier, if they are defined.
// *Type fields can be used to quickly check what is the type of
// the values.
//
// Consult ./parser.go for the supported format
// TODO: This does not handle the zero block correctly
func NewBlockRange(rangeStr string) (*Identifier, error) {
	parsed, err := Parse(rangeStr)
	newRange := &Identifier{}
	if err != nil {
		return nil, handleParserErrors(err)
	}

	newRange.Orig = rangeStr
	newRange.StartType = getPointType(parsed.Points[0])
	newRange.Start = *parsed.Points[0]

	if len(parsed.Points) == 1 {
		newRange.EndType = NotDefined
	} else {
		newRange.EndType = getPointType(parsed.Points[1])
		newRange.End = *parsed.Points[1]
	}

	if parsed.Modifier == nil {
		newRange.ModifierType = NotDefined
	} else {
		newRange.ModifierType = getModifierType(parsed.Modifier)
		newRange.Modifier = *parsed.Modifier
	}

	return newRange, nil
}

func NewTxRange(rangeStr string) (*Identifier, error) {
	parsed, err := Parse(rangeStr)
	if err != nil {
		return nil, handleParserErrors(err)
	}

	// returns at least one point
	newRange := &Identifier{
		Orig:      rangeStr,
		StartType: getPointType(parsed.Points[0]),
		Start:     *parsed.Points[0],
	}

	if len(parsed.Points) == 2 {
		newRange.EndType = getPointType(parsed.Points[1])
		newRange.End = *parsed.Points[1]
	}

	if parsed.Modifier != nil {
		newRange.ModifierType = getModifierType(parsed.Modifier)
		newRange.Modifier = *parsed.Modifier
	}

	return newRange, nil
}

func (id IdentifierType) String() string {
	return []string{
		"NotDefined",
		"BlockNumber",
		"BlockTimestamp",
		"BlockHash",
		"BlockDate",
		"BlockSpecial",
		"TransactionIndex",
		"TransactionHash",
		"Period",
		"Step",
	}[id]
}

func (id *Identifier) UnmarshalJSON(data []byte) error {
	str, err := strconv.Unquote(string(data))
	if err != nil {
		return err
	}

	newBlock, err := NewBlockRange(str)
	if err != nil {
		return err
	}

	*id = *newBlock

	return nil
}

func (id *Identifier) String() string {
	str, _ := json.MarshalIndent(id, "", "  ")
	return string(str)
}

func getPointType(p *Point) IdentifierType {
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

	if p.Number >= utils.EarliestEvmTs {
		return BlockTimestamp
	}

	return BlockNumber
}

func getModifierType(m *Modifier) IdentifierType {
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
