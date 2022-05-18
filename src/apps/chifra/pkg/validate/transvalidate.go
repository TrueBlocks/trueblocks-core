// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package validate

import (
	"strconv"
	"strings"
)

func IsTransHash(str string) bool {
	str = clearDirectional(str)

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
	str = clearDirectional(str)

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
	str = clearDirectional(str)

	parts := strings.Split(str, ".")
	if len(parts) != 2 {
		return false
	}

	validBlockNumber, _ := IsBlockNumber(parts[0])
	return validBlockNumber && IsTransIndex(parts[1])
}

func IsTransBlockHashAndId(str string) bool {
	str = clearDirectional(str)

	parts := strings.Split(str, ".")
	if len(parts) != 2 {
		return false
	}

	return IsBlockHash(parts[0]) && IsTransIndex(parts[1])
}

func IsValidTransId(chain string, ids []string, validTypes ValidArgumentType) (bool, error) {
	err := ValidateIdentifiers(chain, ids, validTypes, 1, nil)
	return err == nil, err
}

func clearDirectional(str string) string {
	if !strings.Contains(str, ":") {
		return str
	}
	// Note that this leaves invalid directional signals which will cause the check to fail
	str = strings.Replace(str, ":next", "", -1)
	str = strings.Replace(str, ":prev", "", -1)
	return str
}
