// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package validate

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
	ValidArgumentTransHash
	ValidArgumentTransBlockNumberAndId
	ValidArgumentTransBlockHashAndId
)

const ValidTransId = ValidArgumentTransHash | ValidArgumentTransBlockNumberAndId | ValidArgumentTransBlockHashAndId
const ValidBlockId = ValidArgumentBlockHash | ValidArgumentBlockNumber | ValidArgumentSpecialBlock
const ValidBlockIdWithRange = ValidBlockId | ValidArgumentRange
const ValidBlockIdWithRangeAndDate = ValidBlockIdWithRange | ValidArgumentDate

// Validates multiple identifiers against multiple valid types (specified as bitmasks).
// If any of the identifiers are invalid, it returns error
// If all identifiers are valid, it returns nil
//
// To check if identifiers are either block number or a date:
// ValidateIdentifiers(identifiers, ValidArgumentBlockNumber | ValidArgumentDate, 0)
//
// To allow for only a single range:
// ValidateIdentifiers(identifiers, ValidArgumentRange, 1)
//
// This routine can be used for both block identifiers and transaction ValidateIdentifiers
//
// ValidateIdentifiers validate block and transaction identifiers
func ValidateIdentifiers(identifiers []string, validTypes ValidArgumentType, maxRanges int) error {
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
			if IsBeforeFirstBlock(identifier) {
				return &InvalidIdentifierLiteralError{
					Value: identifier,
					Msg:   "is before the first block.",
				}
			}
			continue
		}

		if isBitmaskSet(ValidArgumentSpecialBlock) && IsSpecialBlock(identifier) {
			continue
		}

		if isBitmaskSet(ValidArgumentTransHash) && IsTransHash(identifier) {
			continue
		}

		if isBitmaskSet(ValidArgumentTransBlockNumberAndId) && IsTransBlockNumAndId(identifier) {
			continue
		}

		if isBitmaskSet(ValidArgumentTransBlockHashAndId) && IsTransBlockHashAndId(identifier) {
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
