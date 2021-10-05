package validate

import (
	"errors"
	"fmt"
	"regexp"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/blockRange"
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
	_, err := strconv.ParseUint(str, 10, 64)

	return err == nil
}

func IsSpecialBlock(str string) bool {
	// We have no way to tell if a string is valid special block
	// name without calling RPC, so we only check if it looks
	// like a valid name here
	_, err := strconv.Atoi(str)

	if err == nil {
		// numbers not allowed
		return false
	}

	valid, _ := regexp.MatchString(`^[a-z0-9]{3,}$`, str)
	return valid
}

func IsDateTimeString(str string) bool {
	bRange, err := blockRange.New(str)

	if err != nil {
		return false
	}

	return bRange.StartType == blockRange.BlockRangeDate
}

func IsRange(str string) (bool, error) {
	bRange, err := blockRange.New(str)

	if err == nil {
		if bRange.Start.Special == "latest" {
			return false, errors.New("Cannot start range with 'latest'")
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
type ValidArgumentType uint8

const (
	ValidArgumentBlockHash ValidArgumentType = 1 << iota
	ValidArgumentBlockNumber
	ValidArgumentDate
	ValidArgumentRange
	ValidArgumentSpecialBlock
)

const ValidArgumentAll = ValidArgumentBlockHash | ValidArgumentBlockNumber | ValidArgumentDate | ValidArgumentRange | ValidArgumentSpecialBlock

// Errors returned by ValidateBlockIdentifiers (note: it can also return an
// error passed from IsRange)
var ErrTooManyRanges = errors.New("too many ranges")

type InvalidIdentifierLiteralError struct {
	Value string
}

func (e *InvalidIdentifierLiteralError) Error() string {
	return fmt.Sprintf("The given value '%s' is not a numeral or a special named block.", e.Value)
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
