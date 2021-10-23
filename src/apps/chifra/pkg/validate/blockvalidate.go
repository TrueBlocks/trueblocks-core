package validate

import (
	"errors"
	"fmt"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/blockRange"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/specialBlock"
)

func IsBlockHash(str string) bool {
	if !Is0xPrefixed(str) {
		return false
	}

	if len(str) != 66 {
		return false
	}

	if !IsHex(str) {
		return false
	}

	return true
}

func IsBlockNumber(str string) bool {
	base := 10
	source := str

	if Is0xPrefixed(str) {
		base = 16
		source = str[2:]
	}

	_, err := strconv.ParseUint(source, base, 64)

	return err == nil
}

func IsSpecialBlock(str string) bool {
	_, err := strconv.Atoi(str)

	if err == nil {
		// numbers not allowed
		return false
	}

	return specialBlock.IsStringSpecialBlock(str)
}

func IsDateTimeString(str string) bool {
	bRange, err := blockRange.New(str)

	if err != nil {
		return false
	}

	return bRange.StartType == blockRange.BlockRangeDate
}

func IsRange(str string) (bool, error) {
	// Disallow "start only ranges" like "1000" or "london"
	if !strings.Contains(str, "-") {
		return false, &InvalidIdentifierLiteralError{
			Value: str,
		}
	}

	bRange, err := blockRange.New(str)

	if err == nil {
		if bRange.Start.Special == "latest" {
			return false, errors.New("Cannot start range with 'latest'")
		}

		if bRange.StartType == blockRange.BlockRangeSpecial &&
			!specialBlock.IsStringSpecialBlock(bRange.Start.Special) {
			return false, &InvalidIdentifierLiteralError{
				Value: bRange.Start.Special,
			}
		}

		if bRange.EndType == blockRange.BlockRangeSpecial &&
			!specialBlock.IsStringSpecialBlock(bRange.End.Special) {
			return false, &InvalidIdentifierLiteralError{
				Value: bRange.End.Special,
			}
		}

		onlyNumbers := bRange.StartType == blockRange.BlockRangeBlockNumber &&
			bRange.EndType == blockRange.BlockRangeBlockNumber

		if onlyNumbers && bRange.Start.Block > bRange.End.Block {
			return false, errors.New("'stop' must be strictly larger than 'start'")
		}

		return true, nil
	}

	if modifierErr, ok := err.(*blockRange.WrongModifierError); ok {
		return false, errors.New("Input argument appears to be invalid. No such skip marker: " + modifierErr.Token)
	}

	return false, err
}

// Let's define bitmasks to make it easier to validate multiple block identifiers
// (which usually are arguments to `chifra` commands). This way we can specify
// that we want e.g. both block number and a special block as valid arguments
type ValidArgumentType uint16

const (
	ValidArgumentBlockHash ValidArgumentType = 1 << iota
	ValidArgumentBlockNumber
	ValidArgumentDate
	ValidArgumentRange
	ValidArgumentSpecialBlock
)

const ValidBlockId = ValidArgumentBlockHash | ValidArgumentBlockNumber | ValidArgumentSpecialBlock
const ValidBlockIdWithRange = ValidBlockId | ValidArgumentRange
const ValidBlockIdWithRangeAndDate = ValidBlockIdWithRange | ValidArgumentDate

// Errors returned by ValidateBlockIdentifiers (note: it can also return an
// error passed from IsRange)
var ErrTooManyRanges = errors.New("too many ranges")

type InvalidIdentifierLiteralError struct {
	Value string
}

func (e *InvalidIdentifierLiteralError) Error() string {
	return fmt.Sprintf("The given value '%s' is not a numeral or a special named block.", e.Value)
}

// IsValidBlockId returns true or error given a string and a valid block types
func IsValidBlockId(blockId string, validTypes ValidArgumentType) (bool, error) {
	err := ValidateBlockIdentifiers([]string{blockId}, validTypes, 1)
	return err == nil, err
}

// Validates multiple identifiers against multiple valid types (specified as bitmasks).
// If any of the identifiers is invalid, it returns error
// If all identifiers are valid, it returns nil
//
// To check if identifiers are either block number or a date:
// ValidateBlockIdentifiers(identifiers, ValidArgumentBlockNumber | ValidArgumentDate, 0)
//
// To allow for only 1 range:
// ValidateBlockIdentifiers(identifiers, ValidArgumentRange, 1)
func ValidateBlockIdentifiers(identifiers []string, validTypes ValidArgumentType, maxRanges int) error {
	// A helper function to check if bitmask is set
	isBitmaskSet := func(flag ValidArgumentType) bool {
		return validTypes&flag != 0
	}

	rangesFound := 0

	for _, identifier := range identifiers {
		if isBitmaskSet(ValidArgumentBlockHash) && IsBlockHash(identifier) {
			continue
		}

		if isBitmaskSet(ValidArgumentBlockNumber) && IsBlockNumber(identifier) {
			continue
		}

		if isBitmaskSet(ValidArgumentDate) && IsDateTimeString(identifier) {
			continue
		}

		if isBitmaskSet(ValidArgumentSpecialBlock) && IsSpecialBlock(identifier) {
			continue
		}

		// If we are at this point and we don't want a range, it means that
		// an identifier was invalid
		if !isBitmaskSet(ValidArgumentRange) {
			return &InvalidIdentifierLiteralError{
				Value: identifier,
			}
		}

		_, rangeErr := IsRange(identifier)
		if rangeErr != nil {
			return rangeErr
		}

		rangesFound++
		if rangesFound > maxRanges {
			return ErrTooManyRanges
		}
	}

	return nil
}
