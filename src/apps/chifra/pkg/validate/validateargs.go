// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package validate

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

// ValidArgumentType is a bitmask used to make it easier to validate multiple
// block identifiers (which usually are arguments to `chifra` commands). This
// way we can specify that we want e.g. both block number and a special block
// as valid arguments
type ValidArgumentType uint16

const (
	ValidArgumentBlockHash ValidArgumentType = 1 << iota
	ValidArgumentBlockNumber
	ValidArgumentTimestamp
	ValidArgumentDate
	ValidArgumentRange
	ValidArgumentSpecialBlock
	ValidArgumentTransHash
	ValidArgumentTransBlockNumberAndId
	ValidArgumentTransBlockHashAndId
)

const ValidTransId = ValidArgumentTransHash | ValidArgumentTransBlockNumberAndId | ValidArgumentTransBlockHashAndId
const ValidBlockId = ValidArgumentBlockHash | ValidArgumentBlockNumber | ValidArgumentTimestamp | ValidArgumentSpecialBlock
const ValidBlockIdWithRange = ValidBlockId | ValidArgumentRange
const ValidBlockIdWithRangeAndDate = ValidBlockIdWithRange | ValidArgumentDate

// ValidateIdentifiersWithBounds Is a helper function to return bounds in addition to validating identifiers
func ValidateIdentifiersWithBounds(chain string, ids []string, validTypes ValidArgumentType, maxRanges int, results *[]identifiers.Identifier) (base.BlockRange, error) {
	if err := ValidateIdentifiers(chain, ids, validTypes, maxRanges, results); err != nil {
		return base.BlockRange{First: 0, Last: base.NOPOSN}, err
	}
	return identifiers.GetBounds(chain, results)
}

// ValidateIdentifiers validates multiple identifiers against multiple valid types (specified as bitmasks).
// If any of the identifiers are invalid, it returns error
// If all identifiers are valid, it returns nil
//
// Examples:
//
//	ValidateIdentifiers(identifiers, ValidArgumentBlockNumber | ValidArgumentDate, 0)
//	ValidateIdentifiers(identifiers, ValidArgumentRange, 1)
//
// This routine can be used for both block identifiers and transaction
func ValidateIdentifiers(chain string, ids []string, validTypes ValidArgumentType, maxRanges int, results *[]identifiers.Identifier) error {
	// A helper function to check if bitmask is set
	isBitmaskSet := func(flag ValidArgumentType) bool {
		return validTypes&flag != 0
	}

	if isBitmaskSet(ValidTransId) && isBitmaskSet(ValidBlockId) {
		logger.Fatal("should not happen ==> both block ids and transaction ids appear in the same command.")
	}

	rangesFound := 0
	for _, identifier := range ids {
		if isBitmaskSet(ValidArgumentBlockHash) && IsBlockHash(identifier) {
			appendBlockId(results, identifier)
			continue
		}

		validTimestamp, _ := IsTimestamp(identifier)
		if isBitmaskSet(ValidArgumentTimestamp) && validTimestamp {
			appendBlockId(results, identifier)
			continue
		}

		validBlockNumber, _ := IsBlockNumber(identifier)
		if isBitmaskSet(ValidArgumentBlockNumber) && validBlockNumber {
			appendBlockId(results, identifier)
			continue
		}

		if isBitmaskSet(ValidArgumentDate) && IsDateTimeString(identifier) {
			if isBeforeFirstBlock(chain, identifier) {
				return &InvalidIdentifierLiteralError{
					Value: identifier,
					Msg:   "is before the first block.",
				}
			}
			appendBlockId(results, identifier)
			continue
		}

		if isBitmaskSet(ValidArgumentSpecialBlock) && tslib.IsSpecialBlock(chain, identifier) {
			appendBlockId(results, identifier)
			continue
		}

		if isBitmaskSet(ValidArgumentTransHash) && IsTransHash(identifier) {
			appendTxId(results, identifier)
			continue
		}

		if isBitmaskSet(ValidArgumentTransBlockNumberAndId) && IsTransBlockNumAndId(identifier) {
			appendTxId(results, identifier)
			continue
		}

		if isBitmaskSet(ValidArgumentTransBlockHashAndId) && IsTransBlockHashAndId(identifier) {
			appendTxId(results, identifier)
			continue
		}

		// If we are at this point and we don't want a range, it means that
		// an identifier was invalid
		if !isBitmaskSet(ValidArgumentRange) {
			return &InvalidIdentifierLiteralError{
				Value: identifier,
			}
		}

		_, rangeErr := IsRange(chain, identifier)
		if rangeErr != nil {
			return rangeErr
		}

		rangesFound++
		if rangesFound > maxRanges {
			return ErrTooManyRanges
		}
		appendBlockId(results, identifier)
	}

	return nil
}

func appendBlockId(results *[]identifiers.Identifier, identifier string) {
	if results == nil {
		return
	}
	br, _ := identifiers.NewBlockRange(identifier)
	if br != nil {
		*results = append(*results, *br)
	}
}

func appendTxId(results *[]identifiers.Identifier, identifier string) {
	if results == nil {
		return
	}

	// so the parser works for both transaction identifiers and block identifiers
	identifier = strings.Replace(identifier, ".", "-", -1)

	// so the parser works for transaction identifiers with blockHashOrNumber.*
	if strings.Count(identifier, "-") == 1 && strings.HasSuffix(identifier, "-*") {
		parts := strings.Split(identifier, "-")
		identifier = parts[0] + "-0:all"
	}

	br, _ := identifiers.NewTxRange(identifier)
	if br != nil {
		if br.StartType == identifiers.BlockHash && br.EndType == identifiers.NotDefined {
			// We can't really distinguish hashes in the parser, but if there is
			// a single hash and nothing following the separator and we know we're
			// looking for transactions, this is a transaction hash
			br.StartType = identifiers.TransactionHash
		}
		br.EndType = identifiers.TransactionIndex
		*results = append(*results, *br)
	}
}
