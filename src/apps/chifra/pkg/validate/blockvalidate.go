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

import (
	"errors"
	"fmt"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/blockRange"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/specials"
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

	return specials.IsStringSpecialBlock(str)
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
			!specials.IsStringSpecialBlock(bRange.Start.Special) {
			return false, &InvalidIdentifierLiteralError{
				Value: bRange.Start.Special,
			}
		}

		if bRange.EndType == blockRange.BlockRangeSpecial &&
			!specials.IsStringSpecialBlock(bRange.End.Special) {
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

// Errors returned by ValidateIdentifiers (note: it can also return an
// error passed from IsRange)
var ErrTooManyRanges = errors.New("too many ranges")

type InvalidIdentifierLiteralError struct {
	Value string
}

func (e *InvalidIdentifierLiteralError) Error() string {
	return fmt.Sprintf("The given value '%s' is not a numeral or a special named block.", e.Value)
}

func IsValidBlockId(ids []string, validTypes ValidArgumentType) (bool, error) {
	err := ValidateIdentifiers(ids, validTypes, 1)
	return err == nil, err
}
