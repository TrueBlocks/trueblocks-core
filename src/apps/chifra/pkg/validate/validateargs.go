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
