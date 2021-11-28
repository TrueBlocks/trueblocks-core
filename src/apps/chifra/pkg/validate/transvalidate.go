// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package validate

import (
	"strconv"
	"strings"
)

func IsTransHash(str string) bool {
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

func IsTransIndex(str string) bool {
	base := 10
	source := str

	if Is0xPrefixed(str) {
		base = 16
		source = str[2:]
	}

	_, err := strconv.ParseUint(source, base, 64)
	return err == nil
}

func IsTransBlockNumAndId(str string) bool {
	parts := strings.Split(str, ".")
	if len(parts) != 2 {
		return false
	}

	return IsBlockNumber(parts[0]) && IsTransIndex(parts[1])
}

func IsTransBlockHashAndId(str string) bool {
	parts := strings.Split(str, ".")
	if len(parts) != 2 {
		return false
	}

	return IsBlockHash(parts[0]) && IsTransIndex(parts[1])
}

func IsValidTransId(ids []string, validTypes ValidArgumentType) (bool, error) {
	err := ValidateIdentifiers(ids, validTypes, 1)
	return err == nil, err
}
