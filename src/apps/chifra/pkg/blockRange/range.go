package blockRange

import (
	"errors"
	"fmt"
	"regexp"
	"strconv"
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

func (br *BlockRange) UnmarshalJSON(data []byte) error {
	str, err := strconv.Unquote(string(data))

	if err != nil {
		return err
	}

	newBlock, err := New(str)

	if err != nil {
		return err
	}

	*br = *newBlock

	return nil
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
