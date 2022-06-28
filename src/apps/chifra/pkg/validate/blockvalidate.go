// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package validate

import (
	"errors"
	"fmt"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/blockRange"
	tslibPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"

	"github.com/bykof/gostradamus"
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

type blknum_t = uint32

func IsTimestamp(str string) (bool, blknum_t) {
	ok, bn := IsBlockNumber(str)
	if !ok {
		return false, 0
	}
	return bn >= utils.EarliestEvmTs, bn
}

func IsBlockNumber(str string) (bool, blknum_t) {
	base := 10
	source := str

	if Is0xPrefixed(str) {
		base = 16
		source = str[2:]
	}

	value, err := strconv.ParseUint(source, base, 32)
	if err != nil {
		return false, 0
	}

	return true, blknum_t(value)
}

func IsBlockNumberList(strs []string) (bool, []blknum_t) {
	result := make([]blknum_t, len(strs))

	for index, stringValue := range strs {
		check, value := IsBlockNumber(stringValue)
		if !check {
			return false, nil
		}

		result[index] = value
	}

	return true, result
}

func IsDateTimeString(str string) bool {
	if strings.Count(str, "-") != 2 {
		return false
	}

	bRange, err := blockRange.NewBlockRange(str)
	if err != nil {
		return false
	}
	return bRange.StartType == blockRange.BlockDate
}

func ToIsoDateStr2(dateStr string) string {
	// assumes an already validated date string
	str := strings.Replace(dateStr, "T", " ", -1)
	if strings.Count(str, ":") == 0 {
		if strings.Count(str, " ") == 1 {
			str += ":00:00"
		} else {
			str += " 00:00:00"
		}
	} else if strings.Count(str, ":") == 1 {
		str += ":00"
	}
	str = strings.Replace(str, " ", "T", -1)
	str += ".000000"
	return str
}

func isBeforeFirstBlock(chain, dateStr string) bool {
	if !IsDateTimeString(dateStr) {
		return false
	}

	isoStr := ToIsoDateStr2(dateStr)
	dt, _ := gostradamus.Parse(isoStr, gostradamus.Iso8601) // already validated as a date
	firstDate, _ := tslibPkg.FromNameToDate(chain, "0")
	return dt.Time().Before(firstDate.Time())
}

func IsRange(chain, str string) (bool, error) {
	// Disallow "start only ranges" like "1000" or "london"
	if !strings.Contains(str, "-") {
		return false, &InvalidIdentifierLiteralError{
			Value: str,
		}
	}

	bRange, err := blockRange.NewBlockRange(str)

	if err == nil {
		if bRange.Start.Special == "latest" {
			return false, errors.New("cannot start range with 'latest'")
		}

		if bRange.StartType == blockRange.BlockSpecial &&
			!tslibPkg.IsSpecialBlock(chain, bRange.Start.Special) {
			return false, &InvalidIdentifierLiteralError{
				Value: bRange.Start.Special,
			}
		}

		if bRange.EndType == blockRange.BlockSpecial &&
			!tslibPkg.IsSpecialBlock(chain, bRange.End.Special) {
			return false, &InvalidIdentifierLiteralError{
				Value: bRange.End.Special,
			}
		}

		onlyNumbers := bRange.StartType == blockRange.BlockNumber &&
			bRange.EndType == blockRange.BlockNumber

		if onlyNumbers && bRange.Start.Number >= bRange.End.Number {
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
	Msg   string
}

func (e *InvalidIdentifierLiteralError) Error() string {
	if len(e.Msg) == 0 {
		e.Msg = "is not a valid identifier."
	}
	return fmt.Sprintf("The given value '%s' %s", e.Value, e.Msg)
}

func IsValidBlockId(chain string, ids []string, validTypes ValidArgumentType) (bool, error) {
	err := ValidateIdentifiers(chain, ids, validTypes, 1, nil)
	return err == nil, err
}
